#include "Physics.hpp"
#include "PhysicsDebug.hpp"

#include "Scene.hpp"

#define PHYSICS_DEBUG_DRAW

namespace Odysseus2D {

	Physics::Physics(Scene* scene) : m_Scene(scene)
	{
		
	}

	void Physics::OnStart() {
		b2WorldDef worldDef = b2DefaultWorldDef();

		//// 128 pixels per meter is a appropriate for this scene. The boxes are 128 pixels wide.
		//float lengthUnitsPerMeter = 64.0f;
		//b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

		worldDef.gravity = b2Vec2{ 0.0f, 0.0f };
		m_WorldId = b2CreateWorld(&worldDef);

		
#ifdef _DEBUG
		CreateSFMLDebugDraw(m_DebugDraw);
#endif // _DEBUG
		

		auto view = m_Scene->GetRegistry().view<TransformComponent, Rigidbody2DComponent>();
		for (auto [e, transform, rb2d] : view.each())
		{

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			auto pos = transform.Translation + rb2d.CoMOffset;
			bodyDef.position = b2Vec2{ pos.x, pos.y };
			bodyDef.rotation = b2MakeRot(transform.Rotation);
			bodyDef.userData = (void*)(uint64_t)e;


			b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);
			//rb2d.RuntimeBody = (void*) bodyId;
			m_Scene->GetRegistry().emplace<b2BodyId>(e, bodyId);

			if (m_Scene->GetRegistry().all_of<BoxCollider2DComponent>(e))
			{
				auto& bc2d = m_Scene->GetRegistry().get<BoxCollider2DComponent>(e);

				b2Polygon box = b2MakeOffsetBox(bc2d.Size.x * transform.Scale.x / 2, bc2d.Size.y * transform.Scale.y / 2, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), b2MakeRot(0.0f));

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.enableHitEvents = true;
				shapeDef.density = bc2d.Density;
				// shapeDef.enableHitEvents = true;
				//TODO handle offset
				shapeDef.material.friction = bc2d.Friction;
				shapeDef.material.restitution = bc2d.Restitution;
				//TODO set rest thresh

				b2CreatePolygonShape(bodyId, &shapeDef, &box);
			}

			if (m_Scene->GetRegistry().all_of<CircleCollider2DComponent>(e))
			{

				auto& cc2d = m_Scene->GetRegistry().get<CircleCollider2DComponent>(e);

				b2Circle circle;
				circle.center = b2Vec2{ cc2d.Offset.x, cc2d.Offset.y };
				circle.radius = transform.Scale.x * cc2d.Radius;

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = cc2d.Density;
				// shapeDef.enableHitEvents = true;
				//TODO handle offset
				shapeDef.material.friction = cc2d.Friction;
				shapeDef.material.restitution = cc2d.Restitution;
				//TODO set rest thresh

				b2CreateCircleShape(bodyId, &shapeDef, &circle);
			}
		}

#ifdef _DEBUG
		std::cout << "Started PhysWorld of " << m_Scene->DebugName << " Scene with World Id " << m_WorldId.index1 << " " << m_WorldId.generation << std::endl;
#endif // _DEBUG

	}

	void Physics::OnStop() {
		b2DestroyWorld(m_WorldId);
		m_WorldId = b2_nullWorldId;

	}

	void Physics::DestroyBody(entt::entity e)
	{
		auto bodyId = m_Scene->GetRegistry().get<b2BodyId>(e);
		m_Scene->GetRegistry().remove<b2BodyId>(e);
		b2DestroyBody(bodyId);
	}

	void Physics::SetVelocity(entt::entity e, glm::vec2 vel)
	{
		if (m_Scene->GetRegistry().all_of<b2BodyId>(e)) {
			b2BodyId bodyId = m_Scene->GetRegistry().get<b2BodyId>(e);
			b2Body_SetLinearVelocity(bodyId, b2Vec2{ vel.x, vel.y });
		}
	}

	void Physics::ApplyImpulse(entt::entity e, glm::vec2 impulse)
	{
		if (m_Scene->GetRegistry().all_of<b2BodyId>(e)) {
			b2BodyId bodyId = m_Scene->GetRegistry().get<b2BodyId>(e);
			b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2{impulse.x, impulse.y}, true);
		}
	}

	void Physics::SetPosition(entt::entity e, glm::vec2 pos) {
		//TODO
	}





	void Physics::DebugDraw()
	{
#ifdef PHYSICS_DEBUG_DRAW
		b2World_Draw(m_WorldId, &m_DebugDraw);
#endif
	}

	void Physics::OnUpdate(Timestep ts) {
		
		
		float timeStep = 1.0f / 60.f;
		int32_t subSteps = 4;
		b2World_Step(m_WorldId, timeStep, subSteps);
		
		// Retrieve transform from Box2D
		auto view = m_Scene->GetRegistry().view<TransformComponent, Rigidbody2DComponent>();
		for (auto [e, transform, rb2d] : view.each())
		{
			b2BodyId bodyId = m_Scene->GetRegistry().get<b2BodyId>(e);

			const auto& position = b2Body_GetPosition(bodyId);
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation = b2Rot_GetAngle(b2Body_GetRotation(bodyId));
		}


		b2ContactEvents contactEvents = b2World_GetContactEvents(m_WorldId);
		for (int i = 0; i < contactEvents.hitCount; i++) {

			//b2ContactHitEvent hitEvent = contactEvents.hitEvents[i];
			b2ContactHitEvent* hitEvent = contactEvents.hitEvents + i;

			auto ba = b2Shape_GetBody(hitEvent->shapeIdA);
			auto bb = b2Shape_GetBody(hitEvent->shapeIdB);

			entt::entity entA = (entt::entity)(uintptr_t)b2Body_GetUserData(ba);
			entt::entity entB = (entt::entity)(uintptr_t)b2Body_GetUserData(bb);

			Application::Get().GetEventDispatcher().publish<CollisionEvent>({ entA, entB });

		}


		
	}

	Physics::~Physics()
	{
		
	}
}
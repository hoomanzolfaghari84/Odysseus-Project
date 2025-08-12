#include "Scene.hpp"

#include "Odysseus2D/Scene/Components.hpp"
#include "Odysseus2D/Renderer/Renderer2D.hpp"
//#include "Hazel/Physics/Physics2D.h"


namespace Odysseus2D {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		
	}

	std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other) // TODO complete this
	{
		std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;

		// Create entities in new scene
		
		/*for (auto e :)
		{
			
		}*/


		return newScene;
	}

	entt::entity Scene::CreateEntity(const std::string& name)
	{
		auto e = m_Registry.create();
		if (!name.empty()) {
			//TODO create name component
		}
		return e;
	}


	void Scene::DestroyEntity(entt::entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnStart()
	{
		m_IsRunning = true;

		//OnPhysics2DStart();

	}

	void Scene::OnStop()
	{
		m_IsRunning = false;

		//OnPhysics2DStop();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			
			// Physics
			//{
			//	const int32_t velocityIterations = 6;
			//	const int32_t positionIterations = 2;
			//	m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			//	// Retrieve transform from Box2D
			//	auto view = m_Registry.view<Rigidbody2DComponent>();
			//	for (auto e : view)
			//	{
			//		Entity entity = { e, this };
			//		auto& transform = entity.GetComponent<TransformComponent>();
			//		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			//		b2Body* body = (b2Body*)rb2d.RuntimeBody;

			//		const auto& position = body->GetPosition();
			//		transform.Translation.x = position.x;
			//		transform.Translation.y = position.y;
			//		transform.Rotation.z = body->GetAngle();
			//	}
			//}
		}

		// Render 2D
		RenderScene();
		
	}

	//void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	//{
	//	if (!m_IsPaused || m_StepFrames-- > 0)
	//	{
	//		// Physics
	//		{
	//			const int32_t velocityIterations = 6;
	//			const int32_t positionIterations = 2;
	//			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

	//			// Retrieve transform from Box2D
	//			auto view = m_Registry.view<Rigidbody2DComponent>();
	//			for (auto e : view)
	//			{
	//				Entity entity = { e, this };
	//				auto& transform = entity.GetComponent<TransformComponent>();
	//				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

	//				b2Body* body = (b2Body*)rb2d.RuntimeBody;
	//				const auto& position = body->GetPosition();
	//				transform.Translation.x = position.x;
	//				transform.Translation.y = position.y;
	//				transform.Rotation.z = body->GetAngle();
	//			}
	//		}
	//	}

	//	// Render
	//	RenderScene(camera);
	//}

	
	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

	
	entt::entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return entity;
		}
		return entt::null;
	}


	/*void Scene::OnPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}*/

	void Scene::RenderScene()
	{
		Renderer2D::BeginScene();

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite);
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto [entity, transform, circle] : view.each())
			{
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade);
			}
		}

		// Draw text
		{
			/*auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto entity : view)
			{
				auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

				Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)entity);
			}*/
		}

		Renderer2D::EndScene();
	}


}

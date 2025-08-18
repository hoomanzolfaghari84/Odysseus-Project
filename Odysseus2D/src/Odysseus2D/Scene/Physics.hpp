#pragma once

#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "Components.hpp"
#include "Odysseus2D/Core/Timestep.hpp"

namespace Odysseus2D {
	
	class Scene;

	namespace Utils {

		inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
		{
			switch (bodyType)
			{
			case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
			}

			assert(false && "Unknown body type");
			return b2_staticBody;
		}

		inline Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType)
		{
			switch (bodyType)
			{
			case b2_staticBody:    return Rigidbody2DComponent::BodyType::Static;
			case b2_dynamicBody:   return Rigidbody2DComponent::BodyType::Dynamic;
			case b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
			}

			assert(false && "Unknown body type");
			return Rigidbody2DComponent::BodyType::Static;
		}

	}

	struct CollisionEvent
	{
		entt::entity a;
		entt::entity b;

		CollisionEvent(const entt::entity a, const entt::entity b)
			: a(a), b(b)
		{
		}

	};

	class Physics
	{
	public:
		Physics(Scene* scene);
		~Physics();

		void OnStart();
		void OnUpdate(Timestep ts);
		void OnStop();

		void DestroyBody(entt::entity e);

		void SetVelocity(entt::entity e, glm::vec2 vel);
		void ApplyImpulse(entt::entity e, glm::vec2 impulse);
		void SetPosition(entt::entity e, glm::vec2 pos);

		void DebugDraw();
		/*void SetGravity(glm::vec2 gravity) {
			b2World_SetGravity(m_WorldId, gravity);
		}*/

	private:
		b2WorldId m_WorldId = b2_nullWorldId;
		Scene* m_Scene;
		b2DebugDraw m_DebugDraw = { 0 };
	};

}
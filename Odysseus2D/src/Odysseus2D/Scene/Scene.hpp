#pragma once

#include "Odysseus2D/Core/Timestep.hpp"
#include "Physics.hpp"

#include <entt/entt.hpp>

namespace Odysseus2D {


	class Scene
	{
	public:
		Scene();
		~Scene();

		static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);

		entt::entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(entt::entity entity);

		void OnStart();
		void OnStop();

		void OnUpdate(Timestep ts);

		entt::registry & GetRegistry() {
			return m_Registry;
		}

		Physics& GetPhysics() {
			return m_Physics;
		}

		//entt::entity DuplicateEntity(entt::entity entity);

		entt::entity FindEntityByName(std::string_view name);

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

		
	private:
		
		void RenderScene();
	private:
		entt::registry m_Registry;
		Physics m_Physics;

		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		//friend class SceneSerializer;
	};

}

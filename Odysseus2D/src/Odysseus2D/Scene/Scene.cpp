#include "Scene.hpp"

#include "Odysseus2D/Core/Base.hpp"
#include "Odysseus2D/Scene/Components.hpp"
#include "Odysseus2D/Renderer/Renderer2D.hpp"
//#include "Hazel/Physics/Physics2D.h"


namespace Odysseus2D {

	Scene::Scene() : m_Physics(this)
	{
	}

	Scene::~Scene()
	{
	}

	//std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other) // TODO complete this
	//{
	//	std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

	//	auto& srcSceneRegistry = other->m_Registry;
	//	auto& dstSceneRegistry = newScene->m_Registry;

	//	// Create entities in new scene
	//	
	//	/*for (auto e :)
	//	{
	//		
	//	}*/


	//	return newScene;
	//}

	entt::entity Scene::CreateEntity(const std::string& name)
	{
		auto e = m_Registry.create();
		if (!name.empty()) {
			m_Registry.emplace<TagComponent>(e, name);
		}
		return e;
	}


	void Scene::DestroyEntity(entt::entity entity)
	{
		if (m_Registry.all_of<b2BodyId>(entity)) {
			
			m_Physics.DestroyBody(entity);
		}
		m_Registry.destroy(entity);
		entity = entt::null;
		
	}

	void Scene::OnStart()
	{
		m_IsRunning = true;

		OnStartScript();

		m_Physics.OnStart();

#ifdef _DEBUG
		std::cout << "Started " << DebugName << " Scene" << std::endl;
#endif // _DEBUG


	}

	void Scene::OnStop()
	{
		m_IsRunning = false;

		OnStopScript();

		m_Physics.OnStop();

		m_Registry.clear();


#ifdef _DEBUG
		std::cout << "Stopped " << DebugName << " Scene" << std::endl;
#endif // _DEBUG

	}

	void Scene::OnUpdate(Timestep ts)
	{


		OnUpdateScript(ts);

		if (!m_IsRunning) return;

		//if (!m_IsPaused || m_StepFrames-- > 0)
		if(!m_IsPaused)
		{
			
			// Physics
			m_Physics.OnUpdate(ts);
		}

		if (!m_IsRunning) return;
		// Render 2D
		RenderScene();
		
	}
	
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
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto [entity, transform, text] : view.each()) {
				
				Renderer2D::DrawString(text.TextString, transform.GetTransform(), text);
			}
		}

		Renderer2D::EndScene();
		
		m_Physics.DebugDraw();
	}


}

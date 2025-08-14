#include "Gameplay.hpp"

void GameplayLayer::OnAttach()
{
	m_GameplayFactory.LoadAssets();
	Odysseus2D::ConfigManager::GetConfig().Load("assets/game_configs.json");

    
   /* c_BallStartPos = { config["ball"]["start-position"][0].get<float>(), config["ball"]["start-position"][1].get<float>() };
    m_BallEntity = Factory::CreateBall(m_Registry, m_PhysicsSystem, c_BallStartPos);*/

	m_Ball = m_GameplayFactory.CreateBall();
	m_GameplayFactory.CreateBrickGrid();


	m_Scene.OnStart();
}

void GameplayLayer::OnDetach()
{
	m_Scene.OnStop();
}

void GameplayLayer::OnUpdate(Odysseus2D::Timestep ts)
{

	if (Odysseus2D::Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) {

		auto transform = m_Scene.GetRegistry().get<Odysseus2D::TransformComponent>(m_Ball);

		

		m_Scene.GetPhysics().ApplyImpulse(m_Ball, (Odysseus2D::Input::GetMousePosition() - transform.Translation) *= 10);
	}

	m_Scene.OnUpdate(ts);
}

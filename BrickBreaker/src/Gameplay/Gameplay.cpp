#include "Gameplay.hpp"

void GameplayLayer::OnAttach()
{
	m_GameplayFactory.LoadAssets();
	Odysseus2D::ConfigManager::GetConfig().Load("assets/game_configs.json");

    
   /* c_BallStartPos = { config["ball"]["start-position"][0].get<float>(), config["ball"]["start-position"][1].get<float>() };
    m_BallEntity = Factory::CreateBall(m_Registry, m_PhysicsSystem, c_BallStartPos);*/

	m_GameplayFactory.CreateBrickGrid();



}

void GameplayLayer::OnDetach()
{
}

void GameplayLayer::OnUpdate(Odysseus2D::Timestep ts)
{
	m_Scene.OnUpdate(ts);

}

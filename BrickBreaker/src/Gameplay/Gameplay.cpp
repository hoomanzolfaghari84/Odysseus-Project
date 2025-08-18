#include "Gameplay.hpp"



void GameplayLayer::OnAttach()
{
	m_GameplayFactory.LoadAssets();
	Odysseus2D::ConfigManager::GetConfig().Load("assets/game_configs.json");

	m_Ball = m_GameplayFactory.CreateBall();
	m_GameplayFactory.CreateBrickGrid();
	m_Paddle = m_GameplayFactory.CreatePaddle();

	m_ScoreBoard = m_GameplayFactory.CreateScoreBoard();

	m_GameplayFactory.CreateBoundaries();

	auto view =  m_Scene.GetRegistry().view<Odysseus2D::TagComponent>();

	for (auto [e, tag] : view.each()) {
		if (tag.Tag == "Floor") {
			m_Floor = e;
			break;
		}
	}

	Odysseus2D::Application::Get().GetEventDispatcher().subscribe<Odysseus2D::CollisionEvent>([this](const Odysseus2D::CollisionEvent& p) {
		if (m_Scene.GetRegistry().all_of<BrickInfo>(p.a)) {
			DamageBrick(p.a);

		}else if (m_Scene.GetRegistry().all_of<BrickInfo>(p.b)) {
			DamageBrick(p.b);
		}
		else if ((p.a == m_Floor && p.b == m_Ball) || (p.b == m_Floor && p.a== m_Ball) )
		{	
			//TODO Lost ball
		}
		

		});

	m_Scene.OnStart();
}



void GameplayLayer::OnDetach()
{
	m_Scene.OnStop();
}

void GameplayLayer::OnUpdate(Odysseus2D::Timestep ts)
{	
	static glm::vec2 s_BallPosition = {0.f, 0.f};

	auto ballTransform = m_Scene.GetRegistry().get<Odysseus2D::TransformComponent>(m_Ball);

	if (ballTransform.Translation == s_BallPosition)
		m_GameState = AIMING;

	if (m_GameState == AIMING && Odysseus2D::Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) {

		auto transform = m_Scene.GetRegistry().get<Odysseus2D::TransformComponent>(m_Ball);

		m_Scene.GetPhysics().ApplyImpulse(m_Ball, (Odysseus2D::Input::GetMousePosition() - transform.Translation) *= 600);
		m_GameState = PADDLING;
	}

	if (m_GameState == PADDLING) {
		bool left = Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key::Left);
		bool right = Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key::Right);

		if (left xor right) {
			m_Scene.GetPhysics().SetVelocity(m_Paddle, { 200.f * (left ? -1 : 1), 0.f});
		}
		else
		{
			m_Scene.GetPhysics().SetVelocity(m_Paddle, { 0.f, 0.f });
		}
	}
	

	m_Scene.OnUpdate(ts);
}

void GameplayLayer::ResetBall() {

}



void GameplayLayer::DamageBrick(entt::entity brick)
{
	auto& brickInfo = m_Scene.GetRegistry().get<BrickInfo>(brick);

	brickInfo.health--;

	if(brickInfo.health == 0)
	{
		m_Scene.DestroyEntity(brick);
		auto& sci = m_Scene.GetRegistry().get<ScoreBoardInfo>(m_ScoreBoard);
		auto& txt = m_Scene.GetRegistry().get<Odysseus2D::TextComponent>(m_ScoreBoard);
		sci.score += 100;
		txt.TextString = "Score: " + std::to_string(sci.score);
	}
	else if(brickInfo.health == brickInfo.maxHealth/2)
	{
		const Odysseus2D::SpriteSheetData& sheet = Odysseus2D::AssetManager::GetSpriteSheet("BrickBreakerSheet");
		const std::string brickTextureName = GetBrickTextureName(brickInfo.type);

		Odysseus2D::SpriteRendererComponent& sprc = m_Scene.GetRegistry().get<Odysseus2D::SpriteRendererComponent>(brick);
		sprc.Texture = sheet.texture;
		const auto& rect = sheet.sprites.at(brickTextureName+"-cracked").rect;
		sprc.SubRectangle = rect;
	}
}

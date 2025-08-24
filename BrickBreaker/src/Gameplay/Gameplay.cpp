#include "Gameplay.hpp"



void GameplayScene::OnStartScript()
{
	m_GameplayFactory.LoadAssets();
	Odysseus2D::ConfigManager::GetConfig().Load("assets/game_configs.json");

	m_Ball = m_GameplayFactory.CreateBall();
	m_GameplayFactory.CreateBrickGrid();
	m_Paddle = m_GameplayFactory.CreatePaddle();

	m_ScoreBoard = m_GameplayFactory.CreateScoreBoard();

	m_GameplayFactory.CreateBoundaries();

	auto view =  this->GetRegistry().view<Odysseus2D::TagComponent>();

	for (auto [e, tag] : view.each()) {
		if (tag.Tag == "Floor") {
			m_Floor = e;
			break;
		}
	}

	Odysseus2D::Application::Get().GetEventDispatcher().subscribe<Odysseus2D::CollisionEvent>([this](const Odysseus2D::CollisionEvent& p) {
		if (this->GetRegistry().all_of<BrickInfo>(p.a)) {
			DamageBrick(p.a);

		}else if (this->GetRegistry().all_of<BrickInfo>(p.b)) {
			DamageBrick(p.b);
		}
		else if ((p.a == m_Floor && p.b == m_Ball) || (p.b == m_Floor && p.a== m_Ball) )
		{	
			//TODO Lost ball
		}
		

		});
}



void GameplayScene::OnStopScript()
{
	std::cout << "Stopping Gameplay Scene" << std::endl;
}

void GameplayScene::OnUpdateScript(Odysseus2D::Timestep ts)
{	
	static glm::vec2 s_BallPosition = {0.f, 0.f};

	auto ballTransform = this->GetRegistry().get<Odysseus2D::TransformComponent>(m_Ball);

	if (ballTransform.Translation == s_BallPosition)
		m_GameState = AIMING;

	if (m_GameState == AIMING && Odysseus2D::Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) {

		auto transform = this->GetRegistry().get<Odysseus2D::TransformComponent>(m_Ball);

		this->GetPhysics().ApplyImpulse(m_Ball, (Odysseus2D::Input::GetMousePosition() - transform.Translation) *= 600);
		m_GameState = PADDLING;
	}

	if (m_GameState == PADDLING) {
		bool left = Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key::Left);
		bool right = Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key::Right);

		if (left xor right) {
			this->GetPhysics().SetVelocity(m_Paddle, { 200.f * (left ? -1 : 1), 0.f});
		}
		else
		{
			this->GetPhysics().SetVelocity(m_Paddle, { 0.f, 0.f });
		}
	}
	
}

void GameplayScene::ResetBall() {

}



void GameplayScene::DamageBrick(entt::entity brick)
{
	auto& brickInfo = this->GetRegistry().get<BrickInfo>(brick);

	brickInfo.health--;

	if(brickInfo.health == 0)
	{
		this->DestroyEntity(brick);
		auto& sci = this->GetRegistry().get<ScoreBoardInfo>(m_ScoreBoard);
		auto& txt = this->GetRegistry().get<Odysseus2D::TextComponent>(m_ScoreBoard);
		sci.score += 100;
		txt.TextString = "Score: " + std::to_string(sci.score);
	}
	else if(brickInfo.health == brickInfo.maxHealth/2)
	{
		const Odysseus2D::SpriteSheetData& sheet = Odysseus2D::AssetManager::GetSpriteSheet("BrickBreakerSheet");
		const std::string brickTextureName = GetBrickTextureName(brickInfo.type);

		Odysseus2D::SpriteRendererComponent& sprc = this->GetRegistry().get<Odysseus2D::SpriteRendererComponent>(brick);
		sprc.Texture = sheet.texture;
		const auto& rect = sheet.sprites.at(brickTextureName+"-cracked").rect;
		sprc.SubRectangle = rect;
	}
}

#include "Menu.hpp"

void MenuScene::OnStartScript()
{

    auto title = this->CreateEntity("TitleText");
    Odysseus2D::TextComponent trc;
    trc.TextString = "Brick Breaker";
    trc.FontAsset = std::make_shared<sf::Font>("assets\\fonts\\arial\\arial.ttf");
    trc.Color = { 1.f, 1.f, 0.f, 1.f }; // yellow
    this->GetRegistry().emplace<Odysseus2D::TextComponent>(title, trc);
    this->GetRegistry().emplace<Odysseus2D::TransformComponent>(title, glm::vec2{ 400.f, 100.f }, 0.0f, glm::vec2{ 1.f, 1.f });

    auto startButton = this->CreateEntity("StartButton");
    Odysseus2D::TextComponent startTrc;
    startTrc.TextString = "Press ENTER to Start";
    startTrc.FontAsset = std::make_shared<sf::Font>("assets\\fonts\\arial\\arial.ttf");
    startTrc.Color = { 1.f, 1.f, 1.f, 1.f }; // white
    this->GetRegistry().emplace<Odysseus2D::TextComponent>(startButton, startTrc);
    this->GetRegistry().emplace<Odysseus2D::TransformComponent>(startButton, glm::vec2{ 400.f, 200.f }, 0.0f, glm::vec2{ 1.f, 1.f });

}

void MenuScene::OnStopScript()
{

}

void MenuScene::OnUpdateScript(Odysseus2D::Timestep ts)
{

    if (Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key::Enter)) {
        Odysseus2D::Application::Get().GetEventDispatcher().publish<MenuOptionChosen>({START_GAME});
        
    }

}

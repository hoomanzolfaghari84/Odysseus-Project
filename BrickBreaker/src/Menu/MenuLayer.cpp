#include "MenuLayer.hpp"

void MenuLayer::OnAttach()
{

    auto title = m_Scene.CreateEntity("TitleText");
    Odysseus2D::TextComponent trc;
    trc.TextString = "Brick Breaker";
    trc.Color = { 1.f, 1.f, 0.f, 1.f }; // yellow
    m_Scene.GetRegistry().emplace<Odysseus2D::TextComponent>(title, trc);
    m_Scene.GetRegistry().emplace<Odysseus2D::TransformComponent>(title, glm::vec2{ 400.f, 100.f }, 0.0f, glm::vec2{ 1.f, 1.f });

    auto startButton = m_Scene.CreateEntity("StartButton");
    Odysseus2D::TextComponent startTrc;
    startTrc.TextString = "Press ENTER to Start";
    startTrc.Color = { 1.f, 1.f, 1.f, 1.f }; // white
    m_Scene.GetRegistry().emplace<Odysseus2D::TextComponent>(startButton, startTrc);
    m_Scene.GetRegistry().emplace<Odysseus2D::TransformComponent>(startButton, glm::vec2{ 400.f, 200.f }, 0.0f, glm::vec2{ 1.f, 1.f });

    m_Scene.OnStart();
}

void MenuLayer::OnDetach()
{
}

void MenuLayer::OnUpdate(Odysseus2D::Timestep ts)
{
}

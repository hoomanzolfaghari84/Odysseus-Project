#pragma once
#include <Odysseus2D.hpp>
#include "GameplayFactory.hpp"

enum GameState
{
	AIMING,
	PADDLING,
	PAUSED
};

// TODO:: Test if its fater to check ball-brick collisions without the physics engine.


class GameplayLayer : public Odysseus2D::Layer
{
public:
	GameplayLayer() : Layer("Gameplay Layer"), m_GameplayFactory(&m_Scene)
	{
		
	}

	~GameplayLayer()
	{
	}

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Odysseus2D::Timestep ts) override;

private:
	Odysseus2D::Scene m_Scene;
	GameplayFactory m_GameplayFactory;
	GameState m_GameState = AIMING;

	entt::entity m_Ball = entt::null;
	entt::entity m_Paddle = entt::null;
	entt::entity m_Floor = entt::null;
	entt::entity m_ScoreBoard = entt::null;

private:
	void DamageBrick(entt::entity brick);
	void ResetBall();
	
};

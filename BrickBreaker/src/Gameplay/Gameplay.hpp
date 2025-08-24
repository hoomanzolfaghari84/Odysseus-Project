#pragma once
#include <Odysseus2D.hpp>
#include "GameplayFactory.hpp"

enum GameState
{
	AIMING,
	PADDLING,
	PAUSED
};


class GameplayScene : public Odysseus2D::Scene
{
public:
	GameplayScene() : 
#ifdef _DEBUG
		Scene("Gameplay"),
#endif // _DEBUG
		m_GameplayFactory(this)
	{
		
	}

	~GameplayScene()
	{
	}

	void OnStartScript() override;
	void OnStopScript() override;
	void OnUpdateScript(Odysseus2D::Timestep ts) override;

private:
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

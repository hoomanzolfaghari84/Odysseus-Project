#pragma once
#include <Odysseus2D.hpp>
#include "GameplayFactory.hpp"

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
	entt::entity m_Ball = entt::null;
};

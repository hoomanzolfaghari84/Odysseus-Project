#pragma once

#include <Odysseus2D.hpp>



class MenuLayer : public Odysseus2D::Layer
{
public:
	MenuLayer() : Layer("Menu Layer")//, m_MenuFactory(&m_Scene)
	{

	}

	~MenuLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Odysseus2D::Timestep ts) override;

private:
	Odysseus2D::Scene m_Scene;
	//MenuFactory m_MenuFactory;

private:


};

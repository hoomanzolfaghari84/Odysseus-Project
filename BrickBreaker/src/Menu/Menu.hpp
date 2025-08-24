#pragma once

#include <Odysseus2D.hpp>


enum MenusOption
{
	START_GAME,
	EXIT
};

struct MenuOptionChosen {
	MenusOption option;
};


class MenuScene : public Odysseus2D::Scene
{
public:
	MenuScene() : Scene("Menu")//, m_MenuFactory(&m_Scene)
	{

	}

	~MenuScene() = default;

	void OnStartScript() override;
	void OnStopScript() override;
	void OnUpdateScript(Odysseus2D::Timestep ts) override;

private:
	//MenuFactory m_MenuFactory;

private:
	
};

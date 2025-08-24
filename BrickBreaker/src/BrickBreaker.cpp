#include <Odysseus2D.hpp>
#include <Odysseus2D/Core/EntryPoint.hpp>

#include "Gameplay/Gameplay.hpp"
#include "Menu/Menu.hpp"


class BrickBreaker : public Odysseus2D::Application
{


private:
	Odysseus2D::WorldLayer* m_World;

	Odysseus2D::SceneID m_MenuSceneID = 0;
	Odysseus2D::SceneID m_GameplaySceneID = 0;


public:
	BrickBreaker(const Odysseus2D::ApplicationSpecifications& specification)
		: Odysseus2D::Application(specification)
	{
		
		m_World = new Odysseus2D::WorldLayer();

		PushLayer(m_World);
	
		m_MenuSceneID = m_World->RegisterScene(std::make_unique<MenuScene>());
		m_GameplaySceneID = m_World->RegisterScene(std::make_unique<GameplayScene>());

		GetEventDispatcher().subscribe<MenuOptionChosen>([&](MenuOptionChosen e) {

				if (e.option == START_GAME) {
					m_World->SetActiveScene(m_GameplaySceneID);
				}

			});

	}

	~BrickBreaker()
	{
		
	}
};



Odysseus2D::Application* Odysseus2D::CreateApplication(Odysseus2D::ApplicationCommandLineArgs args)
{
	std::cout << "default dir: " <<
		std::filesystem::current_path() << std::endl;

	ApplicationSpecifications spec;
	spec.Name = "BrickBreaker";
	spec.WorkingDirectory = "";
	spec.CommandLineArgs = args;

	if (!spec.WorkingDirectory.empty()) {
		std::filesystem::current_path(spec.WorkingDirectory);
		std::cout << "current dir: " <<
			std::filesystem::current_path() << std::endl;
	}

	return new BrickBreaker(spec);
}


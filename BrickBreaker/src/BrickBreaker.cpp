#include <Odysseus2D.hpp>
#include <Odysseus2D/Core/EntryPoint.hpp>

#include "Gameplay/Gameplay.hpp"
#include "Menu/MenuLayer.hpp"

class BrickBreaker : public Odysseus2D::Application
{

	//MenuLayer* m_MenuLayer = nullptr;
	GameplayLayer* m_GameplayLayer = nullptr;

public:
	BrickBreaker(const Odysseus2D::ApplicationSpecifications& specification)
		: Odysseus2D::Application(specification)
	{

		
		m_GameplayLayer = new GameplayLayer();
		PushLayer(m_GameplayLayer);

		
	}

	~BrickBreaker()
	{
		/*delete m_MenuLayer;
		delete m_GameplayLayer;*/
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


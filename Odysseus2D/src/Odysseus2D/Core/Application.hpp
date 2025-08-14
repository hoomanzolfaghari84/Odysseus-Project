#pragma once

#include <memory>
#include <string>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Odysseus2D/Core/Layer.hpp"
#include "Odysseus2D/Core/LayerStack.hpp"
//#include "Events/EventSystem.hpp"

int main(int argc, char** argv);

namespace Odysseus2D {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			assert(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecifications
	{
		std::string Name = "Odysseus2D Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecifications& specification);
		virtual ~Application();

		/*template<typename E>
		void OnEvent(const E& event) {
			m_EventDispatcher.publish<E>(event);
		}*/

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		static Application& Get() { return *s_Instance; }

		sf::RenderWindow* GetWindow() {
			return m_Window.get();
		}

		const ApplicationSpecifications& GetSpecification() const { return m_Specification; }
		//const EventSystem::EventDispatcher& GetEventDispatcher() const { return m_EventDispatcher; }

	private:
		void Run();
		// bool OnWindowClose(WindowCloseEvent& e);
		// bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationSpecifications m_Specification;
		std::unique_ptr<sf::RenderWindow> m_Window;
		//EventSystem::EventDispatcher m_EventDispatcher;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
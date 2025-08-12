#include "Application.hpp"

#include <SFML/Graphics.hpp>
#include "Timestep.hpp"
#include "Timer.hpp"
#include <Odysseus2D/Renderer/Renderer2D.hpp>

namespace Odysseus2D {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecifications& specs) : m_Specification(specs)
	{

		assert(!s_Instance && "Application already exists!");
		s_Instance = this;


		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = new sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project");

		Renderer2D::Init(m_Window);
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::PushLayer(Layer* layer)
	{

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
		m_Window->close();
	}

	void Application::Run()
	{
		m_Running = true;

		
		m_Window->setFramerateLimit(144);
		Timer timer;
		while (m_Running && m_Window->isOpen())
		{
			
			
			Timestep timestep = timer.Elapsed();
			timer.Reset();
			
			while (const std::optional event = m_Window->pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					Close();
				}
			}

			m_Window->clear();

			if (!m_Minimized)
			{
				{

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);

				}

			}

			m_Window->display();
		}

	}

}
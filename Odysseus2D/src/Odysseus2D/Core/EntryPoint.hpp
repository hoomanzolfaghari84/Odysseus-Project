#pragma once

#include "Application.hpp"

extern Odysseus2D::Application* Odysseus2D::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{

	auto app = Odysseus2D::CreateApplication({ argc, argv });

	app->Run();

	delete app;
}
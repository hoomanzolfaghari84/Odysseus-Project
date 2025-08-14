//#include <Odysseus2D.hpp>
#include "PhysicsDebug.hpp"
#include <iostream>

int main() {

	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{ 0.0f, 10.0f };
	b2WorldId worldId = b2CreateWorld(&worldDef);

	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = b2Vec2{ 10.0f, 10.0f };
	b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
	b2Polygon groundBox = b2MakeBox(5.0f, 1.0f);
	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

	b2DebugDraw debugDraw;

	sf::RectangleShape rect;
	rect.setPosition({ 300.f, 100.f });
	rect.setSize({ 100.f, 20.f });


	auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project (Sandbox)");

	CreateSFMLDebugDraw(debugDraw, window.get());

	window->setFramerateLimit(144);
	//Odysseus2D::Timer timer;

	float pts = 1.0f / 60.0f;
	int subStepCount = 4;

	while (window->isOpen())
	{


		//Odysseus2D::Timestep timestep = timer.Elapsed();
		//timer.Reset();

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}

		}

		window->clear();

		b2World_Step(worldId, pts, subStepCount);
	
		std::cout << "ground position: " << b2Body_GetPosition(groundId).x << " " << b2Body_GetPosition(groundId).y << std::endl;

		b2World_Draw(worldId, &debugDraw);

		window->draw(rect);
		
		window->display();
	}

}
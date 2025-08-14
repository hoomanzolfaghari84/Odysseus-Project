#pragma once

#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

namespace Odysseus2D {

	class Input
	{
	public:
		static bool IsKeyPressed(sf::Keyboard::Key key);

		static bool IsMouseButtonPressed(sf::Mouse::Button button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

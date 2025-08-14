#include "Input.hpp"
#include "Application.hpp"

namespace Odysseus2D {

	bool Odysseus2D::Input::IsKeyPressed(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	bool Odysseus2D::Input::IsMouseButtonPressed(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button);
	}

	glm::vec2 Odysseus2D::Input::GetMousePosition()
	{
		auto pos = sf::Mouse::getPosition();
		return { static_cast<float>(pos.x), static_cast<float>(pos.y)};
	}

	float Odysseus2D::Input::GetMouseX()
	{
		return  sf::Mouse::getPosition().x;
	}

	float Odysseus2D::Input::GetMouseY()
	{
		return  sf::Mouse::getPosition().y;
	}
}


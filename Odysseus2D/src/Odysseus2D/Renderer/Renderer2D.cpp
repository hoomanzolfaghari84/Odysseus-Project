#include "Odysseus2D/Renderer/Renderer2D.hpp"
#include "Odysseus2D/Core/Base.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Odysseus2D {

	sf::RenderWindow* Odysseus2D::Renderer2D::m_Window = nullptr;

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		float LineWidth = 2.0f;

		std::vector<std::pair<sf::RectangleShape,sf::RenderStates>> rects;
		std::vector<std::pair<sf::CircleShape, sf::RenderStates>> circles;
		std::vector<std::pair<sf::Sprite, sf::RenderStates>> sprites;
		
		//std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		//Ref<Texture2D> FontAtlasTexture;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init(sf::RenderWindow* window)
	{
		m_Window = window;

	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene()
	{
		/*sf::Color sfColor = glmVec4ToSfColor({ 0.0,0.0,0.0,0.0 });
		m_Window->clear(sfColor);*/
		StartBatch();
	}

	

	void Renderer2D::EndScene()
	{	
		Flush();
		//m_Window->display();
	}

	void Renderer2D::StartBatch()
	{
		//m_Drawables.clear();
		s_Data.rects.clear();
		s_Data.circles.clear();
		s_Data.sprites.clear();
	}

	void Renderer2D::Flush()
	{

		for (auto rect : s_Data.sprites) {
			m_Window->draw(rect.first, rect.second);
		}

		for (auto rect : s_Data.rects) {
			m_Window->draw(rect.first, rect.second);
		}

		for (auto rect : s_Data.circles) {
			m_Window->draw(rect.first, rect.second);
		}

		
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
		rect.setPosition(glmVec2ToSfVec(position));
		rect.setFillColor(glmVec4ToSfColor(color));
		s_Data.rects.emplace_back(rect, sf::RenderStates::Default);
		//m_Drawables.push_back(rect);
	}

	void Renderer2D::DrawQuad(const glm::mat3& transform, const glm::vec4& color) {
		sf::Transform sfTransform = glmMat3ToSfTransform(transform);
		sf::RectangleShape rect({1.0,1.0});
		rect.setFillColor(glmVec4ToSfColor(color));
		sf::RenderStates states;
		states.transform = sfTransform;
		s_Data.rects.emplace_back(rect, states);

	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
		rect.setPosition(glmVec2ToSfVec(position));
		rect.setRotation(sf::radians(rotation)); // glm rotation is in radians
		rect.setFillColor(glmVec4ToSfColor(color));
		s_Data.rects.emplace_back(rect, sf::RenderStates::Default);
		//m_Drawables.push_back(rect);
	}

	
	void Renderer2D::DrawCircle(const glm::mat3& transform, const glm::vec4& color, float thickness, float fade)
	{
		
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{

	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		/*glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);*/
	}

	void Renderer2D::DrawSprite(const glm::mat3& transform, SpriteRendererComponent& src) // Sprite top left corner is the transform translation
	{
	
		if (src.Texture) {

			sf::Transform sfTransform = glmMat3ToSfTransform(transform);

			sf::Sprite sprite(*src.Texture);

			// If tiling is needed
			auto size = src.Texture->getSize();
			sprite.setTextureRect(src.SubRectangle);

			sprite.setOrigin(glmVec2ToSfVec(src.Origin));

			sprite.setColor(glmVec4ToSfColor(src.Color));
			
			sf::RenderStates states;
			states.transform = sfTransform;

			s_Data.sprites.emplace_back(sprite, states);
		}
		else
		{
			DrawQuad(transform, src.Color);
		}

	}

	/*void Renderer2D::DrawString(const std::string& string, std::shared_ptr<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID)
	{
		
	}

	void Renderer2D::DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component, int entityID)
	{
		DrawString(string, component.FontAsset, transform, { component.Color, component.Kerning, component.LineSpacing }, entityID);
	}*/

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

}

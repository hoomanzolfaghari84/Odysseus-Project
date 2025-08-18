#pragma once
#include <Odysseus2D/Scene/Components.hpp>
#include <SFML/Graphics.hpp>

namespace Odysseus2D {
	class Renderer2D
	{
	public:
		static void Init(sf::RenderWindow* window);
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat3& transform, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		
		static void DrawCircle(const glm::mat3& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawSprite(const glm::mat3& transform, SpriteRendererComponent& src);

		struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		static void DrawString(const std::string& string, std::shared_ptr<sf::Font> font, const glm::mat3& transform, const TextParams& textParams);
		static void DrawString(const std::string& string, const glm::mat3& transform, const TextComponent& component);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();

		static sf::RenderWindow* m_Window;
		//static std::vector<sf::Drawable> m_Drawables;

		static sf::Color glmVec4ToSfColor(const glm::vec4& color)
		{
			return sf::Color(
				static_cast<uint8_t>(color.r * 255.0f),
				static_cast<uint8_t>(color.g * 255.0f),
				static_cast<uint8_t>(color.b * 255.0f),
				static_cast<uint8_t>(color.a * 255.0f)
			);
		}

		static sf::Vector2f glmVec2ToSfVec(const glm::vec2& vec)
		{
			return sf::Vector2f(vec.x, vec.y);
		}

		static sf::Transform glmMat3ToSfTransform(const glm::mat3& mat)
		{
			// GLM stores matrices column-major:
			// mat[0] = first column (vec3)
			// sf::Transform is row-major (it takes 9 floats in row-major order)
			// So we transpose the matrix data

			return sf::Transform(
				mat[0][0], mat[1][0], mat[2][0],
				mat[0][1], mat[1][1], mat[2][1],
				mat[0][2], mat[1][2], mat[2][2]
			);
		}

	};

}
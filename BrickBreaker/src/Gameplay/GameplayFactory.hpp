#pragma once

#include <Odysseus2D.hpp>
#include "Components.hpp"

class GameplayFactory
{
public:
	GameplayFactory(Odysseus2D::Scene* scene) : m_Scene(scene) {};

	void LoadAssets() {
		// Load at game startup
		Odysseus2D::AssetManager::LoadSpriteSheet("BrickBreakerSheet", "assets/assets.json");
	}


	//Ball
	entt::entity CreateBall() {
		const auto& config = Odysseus2D::ConfigManager::GetConfig().GetJson();
		glm::vec2 start_position = { config["ball"]["start-position"][0].get<float>(), config["ball"]["start-position"][1].get<float>() };

		auto entity = m_Scene->CreateEntity("Ball");

		float radius = config["ball"]["radius"].get<float>();

		const Odysseus2D::SpriteSheetData& sheet = Odysseus2D::AssetManager::GetSpriteSheet("BrickBreakerSheet");

		Odysseus2D::SpriteRendererComponent sprc;
		sprc.Texture = sheet.texture;
		const auto& rect = sheet.sprites.at("ball").rect;
		sprc.SubRectangle = rect;
		glm::vec2 scale{
			radius*2 / static_cast<float>(rect.size.x),
			radius*2 / static_cast<float>(rect.size.y)
		};
		sprc.Origin = { static_cast<float>(rect.size.x)/2, static_cast<float>(rect.size.y)/2 };
		m_Scene->GetRegistry().emplace<Odysseus2D::SpriteRendererComponent>(entity, sprc);
		m_Scene->GetRegistry().emplace<Odysseus2D::TransformComponent>(entity, start_position, 0.0f, scale);

		//Physics
		Odysseus2D::Rigidbody2DComponent rb2d;
		rb2d.Type = Odysseus2D::Rigidbody2DComponent::BodyType::Dynamic;
		m_Scene->GetRegistry().emplace<Odysseus2D::Rigidbody2DComponent>(entity, rb2d);
		
		Odysseus2D::CircleCollider2DComponent cc2d;
		cc2d.Restitution = 1.f;
		cc2d.Radius = static_cast<float>(rect.size.x) / 2;
		m_Scene->GetRegistry().emplace<Odysseus2D::CircleCollider2DComponent>(entity, cc2d);


		return entity;
	}


	//Bricks
	void CreateBrickGrid() {
		auto& config = Odysseus2D::ConfigManager::GetConfig().GetJson();

		int screen_width = config["window"]["size"][0].get<int>();
		int screen_height = config["window"]["size"][1].get<int>();

		glm::vec2 brick_size{
			config["brick"]["size"][0].get<float>(),
			config["brick"]["size"][1].get<float>()
		};
		int spacing = config["brick"]["spacing"].get<int>();
		int s = static_cast<int>(brick_size.x) + spacing; // total horizontal space per brick
		int sy = static_cast<int>(brick_size.y) + spacing; // total vertical space per brick

		// Calculate number of bricks that can fully fit
		int num_of_bricks = (screen_width + spacing) / s;
		int num_of_rows = 6;

		// Total grid width in pixels (including spacing between bricks)
		float total_width = num_of_bricks * brick_size.x + (num_of_bricks - 1) * spacing;

		// Margin so grid is horizontally centered
		float margin_x = (screen_width - total_width) / 2.0f;
		// Top margin (could be fixed or from config)
		float margin_y = 50.0f; // for example

		for (int r = 0; r < num_of_rows; r++) {
			for (int c = 0; c < num_of_bricks; c++) {
				// Top-left position of each brick
				glm::vec2 pos = {
					margin_x + c * (brick_size.x + spacing),
					margin_y + r * (brick_size.y + spacing)
				};
				CreateBrick(pos + glm::vec2{brick_size.x/2 , brick_size.y/2}, brick_size);
			}
		}
	}

	entt::entity CreateBrick(glm::vec2 position = { 0.f, 0.f}, glm::vec2 size = {1.f, 1.f}) {
		auto entity = m_Scene->CreateEntity("Brick");

		BrickInfo brickInfo;
		brickInfo.type = (BrickType)Odysseus2D::RandInt(0, BrickType::NumberOfTypes - 1);
		m_Scene->GetRegistry().emplace<BrickInfo>(entity, brickInfo);

		
		const Odysseus2D::SpriteSheetData& sheet = Odysseus2D::AssetManager::GetSpriteSheet("BrickBreakerSheet");
		const std::string brickTextureName = GetBrickTextureName(brickInfo.type);

		Odysseus2D::SpriteRendererComponent sprc;
		sprc.Texture = sheet.texture;
		const auto& rect = sheet.sprites.at(brickTextureName).rect;
		sprc.SubRectangle = rect;
		glm::vec2 scale{ 
			size.x / static_cast<float>(rect.size.x),
			size.y / static_cast<float>(rect.size.y) 
			};
		sprc.Origin = { static_cast<float>(rect.size.x)/2, static_cast<float>(rect.size.y)/2 };
		m_Scene->GetRegistry().emplace<Odysseus2D::SpriteRendererComponent>(entity, sprc);
		m_Scene->GetRegistry().emplace<Odysseus2D::TransformComponent>(entity, position, 0.0f, scale);

		//Physics
		m_Scene->GetRegistry().emplace<Odysseus2D::Rigidbody2DComponent>(entity);
		
		Odysseus2D::BoxCollider2DComponent bc2d;
		bc2d.Size = { rect.size.x, rect.size.y };
		m_Scene->GetRegistry().emplace<Odysseus2D::BoxCollider2DComponent>(entity, bc2d);

		return entity;
	}


	entt::entity CreatePaddle() {
		const auto& config = Odysseus2D::ConfigManager::GetConfig().GetJson();
		glm::vec2 start_position = { config["paddle"]["start-position"][0].get<float>(), config["paddle"]["start-position"][1].get<float>() };

		auto entity = m_Scene->CreateEntity("Ball");

		float size_x = config["paddle"]["size"][0].get<float>();
		float size_y = config["paddle"]["size"][1].get<float>();

		const Odysseus2D::SpriteSheetData& sheet = Odysseus2D::AssetManager::GetSpriteSheet("BrickBreakerSheet");

		Odysseus2D::SpriteRendererComponent sprc;
		sprc.Texture = sheet.texture;
		const auto& rect = sheet.sprites.at("paddle-base").rect;
		sprc.SubRectangle = rect;
		glm::vec2 scale{
			size_x / static_cast<float>(rect.size.x),
			size_y / static_cast<float>(rect.size.y)
		};
		sprc.Origin = { static_cast<float>(rect.size.x) / 2, static_cast<float>(rect.size.y) / 2 };
		m_Scene->GetRegistry().emplace<Odysseus2D::SpriteRendererComponent>(entity, sprc);
		m_Scene->GetRegistry().emplace<Odysseus2D::TransformComponent>(entity, start_position, 0.0f, scale);

		//Physics
		Odysseus2D::Rigidbody2DComponent rb2d;
		rb2d.Type = Odysseus2D::Rigidbody2DComponent::BodyType::Kinematic;
		m_Scene->GetRegistry().emplace<Odysseus2D::Rigidbody2DComponent>(entity, rb2d);

		Odysseus2D::BoxCollider2DComponent bc2d;
		bc2d.Size = { rect.size.x, rect.size.y };
		m_Scene->GetRegistry().emplace<Odysseus2D::BoxCollider2DComponent>(entity, bc2d);

		return entity;
	}


	//entt::entity* CreateBoundaries() {
	void CreateBoundaries() {
		auto& config = Odysseus2D::ConfigManager::GetConfig().GetJson();
		int screen_width = config["window"]["size"][0].get<int>();
		int screen_height = config["window"]["size"][1].get<int>();

		float thickness = 50.0f; // make walls/floor/ceiling thick enough

		// --- Floor ---
		auto e1 = CreateBoundary("Floor",
			{ screen_width / 2.0f, screen_height + thickness / 2.0f },
			{ (float)screen_width, thickness });

		// --- Ceiling ---
		auto e2 = CreateBoundary("Ceiling",
			{ screen_width / 2.0f, -thickness / 2.0f },
			{ (float)screen_width, thickness });

		// --- Left Wall ---
		auto e3 = CreateBoundary("LeftWall",
			{ -thickness / 2.0f, screen_height / 2.0f },
			{ thickness, (float)screen_height });

		// --- Right Wall ---
		auto e4 = CreateBoundary("RightWall",
			{ screen_width + thickness / 2.0f, screen_height / 2.0f },
			{ thickness, (float)screen_height });

		//return new entt::entity[]{e1,e2,e3,e4};
	}

	entt::entity CreateBoundary(const std::string& name, glm::vec2 position, glm::vec2 size) {
		auto entity = m_Scene->CreateEntity(name);

		// Transform
		glm::vec2 scale{ 1.0f, 1.0f };
		m_Scene->GetRegistry().emplace<Odysseus2D::TransformComponent>(entity, position, 0.0f, scale);

		// Physics (static rigidbody, immovable)
		Odysseus2D::Rigidbody2DComponent rb2d;
		rb2d.Type = Odysseus2D::Rigidbody2DComponent::BodyType::Static;
		m_Scene->GetRegistry().emplace<Odysseus2D::Rigidbody2DComponent>(entity, rb2d);

		// Collider (box)
		Odysseus2D::BoxCollider2DComponent bc2d;
		bc2d.Size = size;
		bc2d.Restitution = 1.0f;  // fully bouncy walls
		m_Scene->GetRegistry().emplace<Odysseus2D::BoxCollider2DComponent>(entity, bc2d);

		return entity;
	}

	
	entt::entity CreateScoreBoard() {
		/*const auto& config = Odysseus2D::ConfigManager::GetConfig().GetJson();
		int screen_width = config["window"]["size"][0].get<int>();*/

		auto entity = m_Scene->CreateEntity("ScoreBoard");

		// Starting score component
		ScoreBoardInfo score;
		score.score = 0;
		m_Scene->GetRegistry().emplace<ScoreBoardInfo>(entity, score);

		// Text rendering
		Odysseus2D::TextComponent trc;
		trc.TextString = "Score: 0";
		trc.FontAsset = std::make_shared<sf::Font>("assets\\fonts\\arial\\arial.ttf");
		
		trc.Color = { 1.f, 1.f, 1.f, 1.f }; // white
		m_Scene->GetRegistry().emplace<Odysseus2D::TextComponent>(entity, trc);

		// position at top-center of screen
		glm::vec2 position{ 10.f, 10.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		m_Scene->GetRegistry().emplace<Odysseus2D::TransformComponent>(entity, position, 0.0f, scale);

		return entity;
	}

private:
	Odysseus2D::Scene* m_Scene;

};
#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include "Odysseus2D/Core/Base.hpp"

using json = nlohmann::json;

namespace Odysseus2D {

    struct SpriteDefinition {
        sf::IntRect rect;
    };

    struct AnimationDefinition {
        std::vector<std::string> frames;
        float frameTime = 0.f;
    };

    struct SpriteSheetData {
        std::shared_ptr<sf::Texture> texture; // Pointer to a managed texture
        std::unordered_map<std::string, SpriteDefinition> sprites;
        std::unordered_map<std::string, AnimationDefinition> animations;
    };

    class AssetManager {
    public:
        // Load and store a texture by name
        static std::shared_ptr<sf::Texture> LoadTexture(const std::string& name, const std::string& path) {
            if (textures.find(name) == textures.end()) {
                sf::Texture tex;

                if (tex.loadFromFile(path))
                    textures[name] = std::make_shared<sf::Texture>(tex);
                else return nullptr;
            }
            return textures[name];
        }

        // Get a previously loaded texture
        static std::shared_ptr<sf::Texture> GetTexture(const std::string& name) {
            if (textures.find(name) == textures.end()) {
                throw std::runtime_error("Texture '" + name + "' not loaded!");
            }
            return textures[name];
        }

        // Load a sprite sheet JSON and associate it with a name
        static void LoadSpriteSheet(const std::string& name, const std::string& jsonPath) {
            if (spriteSheets.find(name) != spriteSheets.end()) return;

            SpriteSheetData sheet;

            std::ifstream file(jsonPath);
            if (!file.is_open()) {
                std::cout << "Failed to open assets JSON : " << jsonPath << std::endl;
                throw std::runtime_error("Failed to open assets JSON: " + jsonPath); //TODO Handle gracefully
            }

            json j;
            file >> j;

            std::string texPath = j["texture"];
            std::string textureName = "[sheet]_" + name; // Internal texture ID

            std::cout << "Loading Texture From: " << texPath << std::endl;
            std::shared_ptr<sf::Texture> tex = LoadTexture(textureName, texPath);
            sheet.texture = tex;

            std::cout << "Texture Loaded: " << tex->getSize().x << " " << tex->getSize().y << std::endl;

            for (auto& [key, val] : j["sprites"].items()) {
                sheet.sprites[key] = {
                     sf::IntRect{
                         {val["x"],
                         val["y"]},
                         {val["width"],
                         val["height"]}
                    }
                };

                //LOG("Created Sprite Def: " << sheet.sprites[key].rect.position.x << " " << sheet.sprites[key].rect.position.y << " " << sheet.sprites[key].rect.position)
            }

            for (auto& [key, val] : j["animations"].items()) {
                sheet.animations[key] = {
                    val["frames"].get<std::vector<std::string>>(),
                    val["frame_time"]
                };
            }

            spriteSheets[name] = std::move(sheet);
        }

        // Get a sprite sheet by name
        static SpriteSheetData& GetSpriteSheet(const std::string& name) {
            if (spriteSheets.find(name) == spriteSheets.end()) {
                throw std::runtime_error("Sprite sheet '" + name + "' not loaded!");
            }
            return spriteSheets[name];
        }

        // Clean up all loaded textures
        static void UnloadAll() {
            /*for (auto& [_, tex] : textures) {
                UnloadTexture(tex);
            }*/
            textures.clear();
            spriteSheets.clear(); // texture pointers still valid until above clears
        }

    private:
        static inline std::unordered_map<std::string, std::shared_ptr<sf::Texture> > textures;
        static inline std::unordered_map<std::string, SpriteSheetData> spriteSheets;
    };

} // namespace Engine

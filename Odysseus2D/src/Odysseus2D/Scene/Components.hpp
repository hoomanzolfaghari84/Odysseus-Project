#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Odysseus2D {

    constexpr float deg2rad = 0.0174533;
    constexpr float rad2deg = 57.2958;

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {
        }
    };

    struct TransformComponent
    {
        glm::vec2 Translation = { 0.0f, 0.0f };
        float Rotation = 0.0f; // in radians
        glm::vec2 Scale = { 1.0f, 1.0f };

        glm::mat3 GetTransform() const
        {
            // Build the transform in 2D using mat3
            glm::mat3 translationMat = glm::mat3(1.0f);
            translationMat[2] = glm::vec3(Translation, 1.0f);

            float cosR = cos(Rotation);
            float sinR = sin(Rotation);

            glm::mat3 rotationMat = glm::mat3(
                cosR, sinR, 0.0f,
                -sinR, cosR, 0.0f,
                0.0f, 0.0f, 1.0f
            );

            glm::mat3 scaleMat = glm::mat3(
                Scale.x, 0.0f, 0.0f,
                0.0f, Scale.y, 0.0f,
                0.0f, 0.0f, 1.0f
            );

            return translationMat * rotationMat * scaleMat;
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
        std::shared_ptr<sf::Texture> Texture; // SFML texture
        float TilingFactor = 1.0f;

        sf::IntRect SubRectangle;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(glm::vec4 color)
            : Color(color) {
        }
    };

    struct CircleRendererComponent
    {
        glm::vec4 Color = { 0.0, 0.0, 0.0, 0.0 };
        float Thickness = 1.0f;
        float Fade = 0.005f;

        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;
    };

    struct CameraComponent
    {
        sf::View Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    
    // Physics components (placeholders for now)
    struct Rigidbody2DComponent
    {
        enum class BodyType { Static = 0, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        void* RuntimeBody = nullptr; // link to physics engine body

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };

    struct CircleCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
    };

    struct TextComponent
    {
        std::string TextString;
        std::shared_ptr<sf::Font> FontAsset; // SFML font
        sf::Color Color = sf::Color::White;
        float Kerning = 0.0f;
        float LineSpacing = 0.0f;
    };

    template<typename... Component>
    struct ComponentGroup {};

    using AllComponents =
        ComponentGroup<
        TransformComponent,
        SpriteRendererComponent,
        CircleRendererComponent,
        CameraComponent,
        Rigidbody2DComponent,
        BoxCollider2DComponent,
        CircleCollider2DComponent,
        TextComponent
        >;
}

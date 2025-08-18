#pragma once

#include <Odysseus2D.hpp>

enum BrickType {
    BRICK_DARKBLUE,
    BRICK_LIGHTGREEN,
    BRICK_PURPLE,
    BRICK_RED,
    BRICK_ORANGE,
    BRICK_BLUE,
    BRICK_YELLOW,
    BRICK_DARKGREEN,
    BRICK_GRAY,
    BRICK_BROWN,
    NumberOfTypes
};

inline std::string GetBrickTextureName(BrickType brickType) {
    switch (brickType)
    {
    case BrickType::BRICK_DARKBLUE:
        return "brick-darkblue";
    case BrickType::BRICK_LIGHTGREEN:
        return "brick-lightgreen";
    case BrickType::BRICK_PURPLE:
        return "brick-purple";
    case BrickType::BRICK_RED:
        return "brick-red";
    case BrickType::BRICK_ORANGE:
        return "brick-orange";
    case BrickType::BRICK_BLUE:
        return "brick-blue";
    case BrickType::BRICK_YELLOW:
        return "brick-yellow";
    case BrickType::BRICK_DARKGREEN:
        return "brick-darkgreen";
    case BrickType::BRICK_GRAY:
        return "brick-gray";
    case BrickType::BRICK_BROWN:
        return "brick-brown";
    default:
        return "";
    }
}

struct BrickInfo {
    size_t health = 2;
    BrickType type = BRICK_RED;
    size_t maxHealth = 2;
};

struct BallInfo {
    int speed;
};

struct PaddleInfo
{
    int speed;
};

struct ScoreBoardInfo
{
    int score = 0;
};


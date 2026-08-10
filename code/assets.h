#pragma once
#include "raylib.h"

#include <array>

enum class SpriteId
{
    Player,
    DefaultBlock,
    FireBlock,
    IceBlock,
    JellyBlock,
    Jelly,
    JellyDeathEffect,
    BlockCrushEffect,
    Life,
    Count
};

namespace Assets
{
    static std::array<Texture, static_cast<size_t>(SpriteId::Count)> spriteTextures;

    void LoadAllSprites();
    Texture GetSpriteSheet(SpriteId id);
    void UnloadAllTextures();
}

#pragma once
#include "raylib.h"

#include <array>
#include <cassert>

enum class SpriteId
{
    Player,
    Count
};

namespace Assets
{
    static std::array<Texture, static_cast<size_t>(SpriteId::Count)> spriteTextures;

    void LoadAllSprites();
    Texture GetSpriteSheet(SpriteId id);
    void UnloadAllTextures();
}

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
    Tile,
    TileBorder,
    Doors,
    Title,
    SpaceKey,
    WASD,
    RKey,
    Count
};

enum class SoundId
{
    BlockBreak,
    BlockLand,
    EnemyDeath,
    FrozeEnemy,
    DoorUnlock,
    Footsteps,
    Count
};

namespace Assets
{
    static std::array<Texture, static_cast<size_t>(SpriteId::Count ) + 1> spriteTextures;
    static std::array<Sound, static_cast<size_t>(SoundId::Count) + 1> sounds;
    static Font font;

    void LoadAllSprites();
    void LoadAllSounds();
    Texture& GetSpriteSheet(SpriteId id);
    Sound GetSound(SoundId id);
    void UnloadAllSprites();
    void UnloadAllSounds();
}

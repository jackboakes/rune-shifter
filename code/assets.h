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
    BlockSliding,
    BlockLand,
    EnemyDeath,
    FrozeEnemy,
    DoorUnlock,
    Footsteps,
    Count
};

namespace Assets
{
    extern std::array<Texture, static_cast<size_t>(SpriteId::Count )> spriteTextures;
    extern std::array<Sound, static_cast<size_t>(SoundId::Count)> sounds;
    extern Font font;

    void LoadAllSprites();
    void LoadAllSounds();
    void LoadAllFonts();
    Texture& GetSpriteSheet(SpriteId id);
    Sound GetSound(SoundId id);
    void UnloadAllSprites();
    void UnloadAllSounds();
    void UnloadAllFonts();
}

#include "assets.h"

namespace Assets
{
    void LoadAllSprites()
    {
        spriteTextures[static_cast<size_t>(SpriteId::Player)] = LoadTexture("../data/textures/player_spritesheet.png");
        spriteTextures[static_cast<size_t>(SpriteId::DefaultBlock)] = LoadTexture("../data/textures/default_block.png");
        spriteTextures[static_cast<size_t>(SpriteId::FireBlock)] = LoadTexture("../data/textures/fire_block.png");
        spriteTextures[static_cast<size_t>(SpriteId::IceBlock)] = LoadTexture("../data/textures/ice_block.png");
        spriteTextures[static_cast<size_t>(SpriteId::JellyBlock)] = LoadTexture("../data/textures/jelly_frozen.png");
        spriteTextures[static_cast<size_t>(SpriteId::Jelly)] = LoadTexture("../data/textures/jelly.png");
        spriteTextures[static_cast<size_t>(SpriteId::JellyDeathEffect)] = LoadTexture("../data/textures/jelly_death_effect.png");
        spriteTextures[static_cast<size_t>(SpriteId::BlockCrushEffect)] = LoadTexture("../data/textures/block_crush_effect.png");
        spriteTextures[static_cast<size_t>(SpriteId::Life)] = LoadTexture("../data/textures/life.png");
    }

    Texture GetSpriteSheet(SpriteId id)
    {
        return spriteTextures[static_cast<size_t>(id)];
    }

    void UnloadAllTextures()
    {
        for (Texture& texture : spriteTextures)
        {
            UnloadTexture(texture);
        }
    }
}
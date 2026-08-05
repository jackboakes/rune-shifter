#include "assets.h"

namespace Assets
{
    void LoadAllSprites()
    {
        spriteTextures[static_cast<size_t>(SpriteId::Player)] = LoadTexture("../data/textures/player_spritesheet.png");
        spriteTextures[static_cast<size_t>(SpriteId::DefaultBlock)] = LoadTexture("../data/textures/default_block.png");
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
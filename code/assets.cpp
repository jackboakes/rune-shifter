#include "assets.h"

namespace Assets
{
    void LoadAllSprites()
    {
        spriteTextures[static_cast<size_t>(SpriteId::Player)] = LoadTexture("../data/textures/player_spritesheet.png");
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
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
        spriteTextures[static_cast<size_t>(SpriteId::Tile)] = LoadTexture("../data/textures/tile.png");
        spriteTextures[static_cast<size_t>(SpriteId::TileBorder)] = LoadTexture("../data/textures/tile_border.png");
        spriteTextures[static_cast<size_t>(SpriteId::Doors)] = LoadTexture("../data/textures/doors.png");
        spriteTextures[static_cast<size_t>(SpriteId::Title)] = LoadTexture("../data/textures/title.png");
        spriteTextures[static_cast<size_t>(SpriteId::WASD)] = LoadTexture("../data/textures/wasd.png");
        spriteTextures[static_cast<size_t>(SpriteId::SpaceKey)] = LoadTexture("../data/textures/space.png");
        spriteTextures[static_cast<size_t>(SpriteId::RKey)] = LoadTexture("../data/textures/r.png");
    }

    Texture& GetSpriteSheet(SpriteId id)
    {
        return spriteTextures[static_cast<size_t>(id)];
    }

    void UnloadAllSprites()
    {
        for (Texture& sprite : spriteTextures)
        {
            UnloadTexture(sprite);
        }
    }

    void LoadAllSounds()
    {
        sounds[static_cast<size_t>(SoundId::BlockBreak)] = LoadSound("../data/sound/block_break.mp3");
        sounds[static_cast<size_t>(SoundId::BlockLand)] = LoadSound("../data/sound/block_land.mp3");
        sounds[static_cast<size_t>(SoundId::EnemyDeath)] = LoadSound("../data/sound/jelly_death.mp3");
        sounds[static_cast<size_t>(SoundId::FrozeEnemy)] = LoadSound("../data/sound/froze_enemy.mp3");
        sounds[static_cast<size_t>(SoundId::DoorUnlock)] = LoadSound("../data/sound/door_unlock.mp3");
        sounds[static_cast<size_t>(SoundId::Footsteps)] = LoadSound("../data/sound/footsteps.mp3");
    }

    Sound GetSound(SoundId id)
    {
        return sounds[static_cast<size_t>(id)];
    }

    void UnloadAllSounds()
    {
        for (Sound& sound : sounds)
        {
            UnloadSound(sound);
        }
    }
}
#pragma once

#include "raylib.h"

#include "common/types.h"

#include <array>

enum class ProgramMode
{
    Game,
    Menu,
    Victory
};
inline ProgramMode g_ProgramMode { ProgramMode::Game };

//=================================================================
// NOTE:: Screen Globals
//=================================================================

inline constexpr S32 g_ScreenWidth { 720 };
inline constexpr S32 g_ScreenHeight { 720 };
inline constexpr S32 g_RenderTextureWidth { 360 };
inline constexpr S32 g_RenderTextureHeight { 360 };
inline RenderTexture2D g_Target = { 0 };

//=================================================================
// NOTE:: Sprite Animation
//=================================================================

struct SpriteAnimation
{
    /*
    The number of animation frames in a sprite sheet row or column
    */
    U32 frameCount { 1 };
    /*
    The frame the animation is currently showing
    */
    U32 currentFrame { 0 };
    /*
    The vertical width of an individual animation frame
    */
    U32 frameWidth { 0 };
    /*
    The vertical height of an individual animation frame
    */
    U32 frameHeight { 0 };
    /*
    The amount of horizontal and vertical padding in a sprite sheet
    */
    U32 padding { 24 };
    /*
    The number of ticks that advance an animations frame.
    1 means one animation frame per tick,
    so at 60 ticks/sec that's 60 animation frames per second.
    30 means 30 frames before a new animation frame.
    */
    U32 frameAdvancement { 1 };
    /*
    Defines which row the animation is playing
    */
    U32 row { 0 };
};

//=================================================================
// NOTE:: Entity
//=================================================================

enum class EntityKind
{
    None = 0,
    Player,
    Block,
    Fire,
    Wall,
    Enemy,
    Effect,
    Door
};

struct EntityHandle
{
    U64 id { 0 };
    U64 index { 0 };
};

enum class Direction
{
    None = 0,
    Up,
    Down,
    Left,
    Right
};

enum class BlockKind
{
    None = 0,
    Default,
    Fire,
    Ice,
    FrozenEnemy
};

struct Entity
{
    EntityHandle handle;
    EntityKind kind { EntityKind::None };
    BlockKind blockKind { BlockKind::None };

    B32 pushed;
    B32 lockState;

    Texture texture;
    SpriteAnimation animation;

    IVector2 gridPosition;
    IVector2 targetGridPosition;
    Vector2 startPosition;
    Vector2 position;
    Vector2 targetPosition;
    F32 positionT { 0.0f };

    F32 speed;
    Direction direction;
};

//=================================================================
// NOTE:: Tilemap
//=================================================================

inline constexpr U32 g_TileMapCountX { 15 };
inline constexpr U32 g_TileMapCountY { 15 };
inline constexpr U32 g_TileSize { 24 };

enum class TileCorner
{
    None,
    TopLeft,
    TopRight,
    BottomRight,
    BottomLeft
};

/*
NOTE::
0 = void
1 = wall
2 = floor
*/
inline constexpr U32 level1[g_TileMapCountY][g_TileMapCountX]
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
};

inline constexpr U32 level2[g_TileMapCountY][g_TileMapCountX]
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,2,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,1,2,2,2,2,2,2,1,0,0,0,0,0},
    {0,0,1,2,2,1,1,2,2,1,0,0,0,0,0},
    {0,0,1,1,1,1,1,2,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

inline constexpr U32 level3[g_TileMapCountY][g_TileMapCountX]
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,2,1,1,0,0,0,0,0},
    {0,1,2,2,2,2,2,2,2,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,2,2,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,2,1,1,1,1,0,0,0},
    {0,0,0,1,2,2,2,2,2,2,2,1,0,0,0},
    {0,0,0,1,2,2,2,2,2,2,2,1,0,0,0},
    {0,0,0,1,2,2,2,2,2,2,2,1,0,0,0},
    {0,0,0,1,2,2,2,2,2,2,2,1,0,0,0},
    {0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
};

inline constexpr U32 level4[g_TileMapCountY][g_TileMapCountX]
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,0,1,1,1,0,1,1,0},
    {0,0,0,1,2,2,1,1,1,2,2,2,2,1,0},
    {0,0,0,1,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,0,1,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,0,1,1,1,2,1,1,1,1,2,1,1,0},
    {0,0,0,0,0,1,2,1,0,0,1,2,1,0,0},
    {0,0,0,0,0,1,2,1,0,0,1,2,1,0,0},
    {0,0,1,1,1,1,2,1,1,1,1,2,1,1,0},
    {0,0,1,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,1,2,2,2,2,2,2,2,1,1,2,1,0},
    {0,0,1,2,2,2,2,2,2,2,1,1,2,1,0},
    {0,0,1,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,1,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
};

struct TileMap
{
    IVector2 count;
    U32 tileSize;
    const U32(*tiles)[g_TileMapCountX];
};

//=================================================================
// NOTE:: Game
//=================================================================

enum class Level : U32
{
    Level1,
    Level2,
    Level3,
    Level4,
    Count
};

inline constexpr U32 g_MaxEntities { 256 };

struct GameState
{
    U64 tick;
    std::array<Entity, g_MaxEntities> entities;
    U64 entityCount { 1 };
    std::array<U64, g_MaxEntities> freeList;
    U64 freeCount { 0 };
    U64 nextHandleId { 1 };
    EntityHandle playerHandle;

    Level level { Level::Level1 };
    U32 maxLevels { static_cast<U32>(Level::Count) };

    Direction tapBuffer { Direction::None };

    TileMap tileMap;

    Font font;
};

namespace Game
{
    void Initialise(GameState& gameState);
    static void Update(GameState& gameState, F32 dt);
    static void DrawFrame(GameState& gameState);
    void UpdateAndDrawFrame(GameState& gameState, F32 dt);
    void Unload();
}

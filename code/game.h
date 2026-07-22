#pragma once

#include "raylib.h"

#include "common/types.h"

#include <array>

enum class ProgramMode
{
    Game,
    Menu
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
// NOTE:: Entity
//=================================================================

enum class EntityKind
{
    None = 0,
    Player,
    Block,
    Fire,
    Wall,
    Enemy
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

struct Entity
{
    EntityHandle handle;
    EntityKind kind { EntityKind::None };

    IVector2 gridPosition;
    IVector2 targetGridPosition;
    Vector2 startPosition;
    Vector2 position;
    Vector2 targetPosition;
    F32 positionT { 0.0f };

    Vector2 velocity;
    F32 speed;
    Direction direction;
};

//=================================================================
// NOTE:: Tilemap
//=================================================================

inline constexpr U32 g_TileMapCountX { 15 };
inline constexpr U32 g_TileMapCountY { 15 };
inline constexpr U32 g_TileSize { 24 };


/*
NOTE::
0 = void
1 = wall
2 = floor
*/
inline constexpr U32 tiles[g_TileMapCountY][g_TileMapCountX]
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

struct TileMap
{
    IVector2 count;
    U32 tileSize;
    const U32(*tiles)[g_TileMapCountX];
};

//=================================================================
// NOTE:: GameState
//=================================================================

inline constexpr U32 g_MaxEntities { 256 };

struct GameState
{
    std::array<Entity, g_MaxEntities> entities;
    U64 entityCount { 1 };
    EntityHandle playerHandle;

    TileMap tileMap;
};

namespace Game
{
    void Initialise(GameState& gameState);
    static void Update(GameState& gameState, F32 dt);
    static void DrawFrame(GameState& gameState);
    void UpdateAndDrawFrame(GameState& gameState, F32 dt);
}

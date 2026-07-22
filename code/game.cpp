#include "game.h"
#include "raymath.h"
#include <algorithm>
#include <cassert>

Vector2 Vector2FromDirection(Direction direction)
{
    Vector2 directionVector { 0.0f,0.0f };
    switch (direction)
    {
    case Direction::Up:
    {
        directionVector = { 0.0f, -1.0f };
    }
    break;
    case Direction::Down:
    {
        directionVector = { 0.0f, 1.0f };
    }
    break;
    case Direction::Left:
    {
        directionVector = { -1.0f, 0.0f };
    }
    break;
    case Direction::Right:
    {
        directionVector = { 1.0f, 0.0f };
    }
    break;
    }

    return directionVector;

}

Vector2 WorldPositionFromGridPosition(IVector2 gridPosition)
{
    return { static_cast<F32>(gridPosition.x) * static_cast<F32>(g_TileSize), static_cast<F32>(gridPosition.y) * static_cast<F32>(g_TileSize) };
}

void DrawTileMap(const TileMap& tileMap)
{
    for (int row { 0 }; row < tileMap.count.y; row++)
    {
        for (int column { 0 }; column < tileMap.count.x; column++)
        {
            IVector2 gridPosition { column, row };
            Vector2 position { WorldPositionFromGridPosition(gridPosition) };
            if (tileMap.tiles[row][column] == 0)
            {
                continue;
            }
            else if(tileMap.tiles[row][column] == 1)
            {
                DrawRectangle(position.x, position.y, tileMap.tileSize, tileMap.tileSize, WHITE);
            }
            else if (tileMap.tiles[row][column] == 2)
            {
                DrawRectangle(position.x, position.y, tileMap.tileSize, tileMap.tileSize, BLUE);
                DrawRectangleLines(position.x, position.y, tileMap.tileSize, tileMap.tileSize, BLACK);
            }
        }
    }
}

//=================================================================
// NOTE:: Menu Implementation
//=================================================================

void ToggleMenu()
{
    if (g_ProgramMode == ProgramMode::Game)
    {
        g_ProgramMode = ProgramMode::Menu;
    }
    else
    {
        g_ProgramMode = ProgramMode::Game;
    }

}

void DrawMenu()
{
    DrawText("Draw Menu", 0, 0, 16, WHITE);
}

//=================================================================
// NOTE:: Game
//=================================================================

Entity* EntityFromHandle(GameState& gameState, EntityHandle handle)
{
    Entity* entity { &gameState.entities[handle.index] };
    if (entity->handle.id == handle.id)
    {
        return entity;
    }

    return nullptr;
}

EntityHandle AddEntity(GameState& gameState)
{
    U64 entityIndex { gameState.entityCount++ };

    assert(gameState.entityCount < gameState.entities.size() && "Entity count greater than size at add entity");
    //TODO:: get an old index from the free list once we add that
    return { entityIndex, entityIndex };
}

void AddPlayer(GameState& gameState)
{
    EntityHandle handle = AddEntity(gameState);
    Entity player;
    player.kind = EntityKind::Player;
    player.direction = Direction::Down;
    player.speed = 80.0f;
    player.handle = handle;
    player.gridPosition = { 7,7 };
    player.targetGridPosition = player.gridPosition;
    player.position = WorldPositionFromGridPosition(player.gridPosition);
    player.targetPosition = player.position;
    player.startPosition = player.position;
    gameState.entities[handle.index] = player;

    gameState.playerHandle = player.handle;
}

void StartMove(GameState& gameState, EntityHandle playerHandle, Direction newDirection, IVector2 gridMove)
{
    Entity* entity { EntityFromHandle(gameState, playerHandle) };
    if (entity)
    {
        // Block movement if already moving
        if (entity->position != entity->targetPosition)
        {
            return;
        }

        if (newDirection != Direction::None)
        {
            entity->direction = newDirection;
        }

        entity->startPosition = entity->position;
        entity->targetGridPosition.x =  entity->gridPosition.x + gridMove.x;
        entity->targetGridPosition.y = entity->gridPosition.y + gridMove.y;
        entity->targetPosition = WorldPositionFromGridPosition(entity->targetGridPosition);
        entity->positionT = 0.0f;
    }
}

void MovePlayer(GameState& gameState, EntityHandle playerHandle, F32 dt)
{
    // TODO:: check if grid move is possible
    Entity* player { EntityFromHandle(gameState, playerHandle) };
    if (player)
    {
        F32 distance { Vector2Length(player->targetPosition - player->startPosition) };
        if (distance <= 0.0f)
        {
            return;
        }
        player->positionT += (player->speed * dt) / distance;

        if (player->positionT >= 0.5f)
        {
            player->gridPosition = player->targetGridPosition;
        }

        if (player->positionT >= 1.0f)
        {
            player->positionT = 1.0f;
            player->position = player->targetPosition;
            player->gridPosition = player->targetGridPosition;
        }
        else
        {
            player->position = Vector2Lerp(player->startPosition, player->targetPosition, player->positionT);
        }
    }
    else
    {
        assert(player && "Player not found when moving");
    }
}

void DrawPlayer(Entity& player)
{
    DrawRectangle(player.position.x, player.position.y, g_TileSize, g_TileSize, YELLOW);
}

void DrawGame(GameState& gameState)
{
    DrawTileMap(gameState.tileMap);
    Entity* player { EntityFromHandle(gameState, gameState.playerHandle) };
    if (player)
    {
        DrawPlayer(*player);
    }
    else
    {
        assert(!player && "Player not found when drawing");
    }
    
}

namespace Game
{
    void Initialise(GameState& gameState)
    {
        gameState.tileMap.count = { g_TileMapCountX, g_TileMapCountY };
        gameState.tileMap.tileSize = g_TileSize;
        gameState.tileMap.tiles = tiles;

        AddPlayer(gameState);
    }
    
    static void Update(GameState& gameState, F32 dt)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ToggleMenu();
        }

        switch (g_ProgramMode)
        {
        case ProgramMode::Menu:
        {

        }
        break;
        case ProgramMode::Game:
        {
            IVector2 gridMove { 0,0 };
            Direction direction { Direction::None };
            if (IsKeyDown(KEY_W))
            {
                gridMove.y -= 1;
                direction = Direction::Up;
            }
            else if (IsKeyDown(KEY_A))
            {
                gridMove.x -= 1;
                direction = Direction::Left;
            }
            else if (IsKeyDown(KEY_S))
            {
                gridMove.y += 1;
                direction = Direction::Down;
            }
            else if (IsKeyDown(KEY_D))
            {
                gridMove.x += 1;
                direction = Direction::Right;
            }

            if (gridMove.x != 0 || gridMove.y != 0)
            {
                StartMove(gameState, gameState.playerHandle, direction, gridMove);
            }
            
            MovePlayer(gameState, gameState.playerHandle, dt);
        }
        break;
        }
    }

    static void DrawFrame(GameState& gameState)
    {
        BeginTextureMode(g_Target);
            ClearBackground(BLACK);

            switch (g_ProgramMode)
            {
            case ProgramMode::Game:
            {
                DrawGame(gameState);
            }
            break;
            case ProgramMode::Menu:
            {
                DrawMenu();
            }
            break;
            }

        EndTextureMode();
        // NOTE:: Render to the main buffer
        BeginDrawing();
        ClearBackground(MAGENTA);
        // draw render texture scaled
        {
            F32 screenWidth { static_cast<F32>(GetScreenWidth()) };
            F32 screenHeight { static_cast<F32>(GetScreenHeight()) };
            F32 renderTargetWidth { static_cast<F32>(g_RenderTextureWidth) };
            F32 renderTargetHeight { static_cast<F32>(g_RenderTextureHeight) };
            F32 scale { std::min(screenWidth / renderTargetWidth, screenHeight / renderTargetHeight) };
            F32 destinationWidth { renderTargetWidth * scale };
            F32 destinationHeight { renderTargetHeight * scale };
            Rectangle destination { (screenWidth - destinationWidth) * 0.5f, (screenHeight - destinationHeight) * 0.5f, destinationWidth, destinationHeight };

            DrawTexturePro(g_Target.texture, { 0, 0, renderTargetWidth, -renderTargetHeight }, destination, { 0, 0 }, 0.0f, WHITE);
        }
        EndDrawing();
    }

    void UpdateAndDrawFrame(GameState& gameState, F32 dt)
    {
        Update(gameState, dt);
        DrawFrame(gameState);
    }
}
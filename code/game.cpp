#include "game.h"

#include "raymath.h"

#include "assets.h"

#include <algorithm>
#include <cassert>

IVector2 IVector2FromDirection(Direction direction)
{
    IVector2 directionVector { 0 , 0 };
    switch (direction)
    {
    case Direction::None:
    {
        directionVector = { 0 , 0 };
    }
    break;
    case Direction::Up:
    {
        directionVector = { 0 , -1 };
    }
    break;
    case Direction::Down:
    {
        directionVector = { 0 , 1 };
    }
    break;
    case Direction::Left:
    {
        directionVector = { -1 , 0 };
    }
    break;
    case Direction::Right:
    {
        directionVector = { 1 , 0 };
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
    Entity entity;
    entity.kind = EntityKind::Player;
    entity.texture = Assets::GetSpriteSheet(SpriteId::Player);
    entity.animation.frameCount = 4;
    entity.animation.currentFrame = 0;
    entity.animation.frameWidth = 24;
    entity.animation.frameHeight = 24;
    entity.animation.frameAdvancement = 6;
    entity.direction = Direction::Down;
    entity.speed = 80.0f;
    entity.handle = handle;
    entity.gridPosition = { 7,7 };
    entity.targetGridPosition = entity.gridPosition;
    entity.position = WorldPositionFromGridPosition(entity.gridPosition);
    entity.targetPosition = entity.position;
    entity.startPosition = entity.position;
    gameState.entities[handle.index] = entity;

    gameState.playerHandle = entity.handle;
}

B32 StartMove(GameState& gameState, EntityHandle playerHandle, Direction newDirection, IVector2 gridMove)
{
    B32 result { false };
    Entity* entity { EntityFromHandle(gameState, playerHandle) };
    if (entity)
    {
        B32 isMoving { entity->position != entity->targetPosition };
        if (!isMoving)
        {
            if (newDirection != Direction::None)
            {
                entity->direction = newDirection;
            }

            entity->startPosition = entity->position;
            entity->targetGridPosition.x = entity->gridPosition.x + gridMove.x;
            entity->targetGridPosition.y = entity->gridPosition.y + gridMove.y;
            entity->targetPosition = WorldPositionFromGridPosition(entity->targetGridPosition);
            entity->positionT = 0.0f;

            result = true;
        }
    }
    return result;
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

        // TODO:: find a bettter palce to update animation
        if (gameState.tick % player->animation.frameAdvancement == 0)
        {
            player->animation.currentFrame = (player->animation.currentFrame + 1) % player->animation.frameCount;
        }
    }
    else
    {
        assert(player && "Player not found when moving");
    }
}

void DrawPlayer(Entity& entity)
{
    U32 column { 0 };
    switch (entity.direction)
    {
    case Direction::Right: column = 0; break;
    case Direction::Up:    column = 1; break;
    case Direction::Left:  column = 2; break;
    case Direction::Down:  column = 3; break;
    }

    U32 row { entity.animation.currentFrame };
    U32 padding { entity.animation.padding };
    U32 width { entity.animation.frameWidth };
    U32 height { entity.animation.frameHeight };

    Rectangle source;
    source.x = static_cast<F32>(padding + (column * (width + padding)));
    source.y = static_cast<F32>(padding + (row * (height + padding)));
    source.width = static_cast<F32>(width);
    source.height = static_cast<F32>(height);

    DrawTextureRec(entity.texture, source, entity.position, WHITE);

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

        Assets::LoadAllSprites();

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
            Direction hold { Direction::None };

            if (IsKeyDown(KEY_W)) hold = Direction::Up;
            if (IsKeyDown(KEY_A)) hold = Direction::Left;
            if (IsKeyDown(KEY_S)) hold = Direction::Down;
            if (IsKeyDown(KEY_D)) hold = Direction::Right;

            if (IsKeyPressed(KEY_W)) gameState.tapBuffer = Direction::Up;
            if (IsKeyPressed(KEY_A)) gameState.tapBuffer = Direction::Left;
            if (IsKeyPressed(KEY_S)) gameState.tapBuffer = Direction::Down;
            if (IsKeyPressed(KEY_D)) gameState.tapBuffer = Direction::Right;

            // NOTE:: Prioritise the tap buffer input otherwise fall back to the held input direction
            Direction direction { Direction::None };
            if (gameState.tapBuffer != Direction::None)
            {
                direction = gameState.tapBuffer;
            }
            else
            {
                direction = hold;
            }

            IVector2 gridMove { IVector2FromDirection(direction) };
            if (gridMove.x != 0 || gridMove.y != 0)
            {
                B32 started { StartMove(gameState, gameState.playerHandle, direction, gridMove) };
                if (started)
                {
                    gameState.tapBuffer = Direction::None;
                }
                
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
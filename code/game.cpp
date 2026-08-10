#include "game.h"

#include "raymath.h"

#include "assets.h"

#include <algorithm>
#include <cassert>
#include <cmath>

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
    for (S32 row { 0 }; row < tileMap.count.y; row++)
    {
        for (S32 column { 0 }; column < tileMap.count.x; column++)
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

Entity* EntityFromGridPosition(GameState& gameState, EntityKind kind, IVector2 gridPosition)
{
    Entity* entity { nullptr };
    for (Entity& e : gameState.entities)
    {
        if (e.kind != kind) continue;
        if (e.gridPosition.x == gridPosition.x && e.gridPosition.y == gridPosition.y)
        {
            entity = &e;
            break;
        }
    }

    return entity;
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
    EntityHandle handle { AddEntity(gameState) };

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

void AddBlock(GameState& gameState, BlockKind blockKind, IVector2 gridPosition)
{
    EntityHandle handle { AddEntity(gameState) };

    Texture texture;

    switch (blockKind)
    {
    case BlockKind::Default:
    {
        texture = Assets::GetSpriteSheet(SpriteId::DefaultBlock);
    }
    break;
    case BlockKind::Fire:
    {
        texture = Assets::GetSpriteSheet(SpriteId::FireBlock);
    }
    break;
    case BlockKind::Ice:
    {
        texture = Assets::GetSpriteSheet(SpriteId::IceBlock);
    }
    break;
    case BlockKind::FrozenEnemy:
    {
        texture = Assets::GetSpriteSheet(SpriteId::JellyBlock);
    }
    break;
    }

    Entity entity;
    entity.handle = handle;
    entity.kind = EntityKind::Block;
    entity.blockKind = blockKind;
    entity.pushed = false;
    entity.texture = texture;
    entity.animation.frameCount = 6;
    entity.animation.currentFrame = 0;
    entity.animation.frameWidth = 24;
    entity.animation.frameHeight = 24;
    entity.animation.frameAdvancement = 8;
    entity.speed = 360.0f;

    entity.gridPosition = gridPosition;
    entity.targetGridPosition = entity.gridPosition;
    entity.position = WorldPositionFromGridPosition(entity.gridPosition);
    entity.targetPosition = entity.position;
    entity.startPosition = entity.position;
    gameState.entities[handle.index] = entity;
}

void AddEffect(GameState& gameState, SpriteId spriteId, Vector2 worldPosition, U32 frameCount, U32 frameAdvancement)
{
    EntityHandle handle { AddEntity(gameState) };

    Entity entity;
    entity.handle = handle;
    entity.kind = EntityKind::Effect;
    entity.texture = Assets::GetSpriteSheet(spriteId);
    entity.position = worldPosition;

    entity.animation.frameCount = frameCount;
    entity.animation.currentFrame = 0;
    entity.animation.frameWidth = 24;
    entity.animation.frameHeight = 24;
    entity.animation.frameAdvancement = frameAdvancement;
    entity.animation.row = 0;

    gameState.entities[handle.index] = entity;
}

void AddEnemy(GameState& gameState, IVector2 gridPosition)
{
    EntityHandle handle { AddEntity(gameState) };

    Entity entity;
    entity.handle = handle;
    entity.kind = EntityKind::Enemy;
    entity.texture = Assets::GetSpriteSheet(SpriteId::Jelly);
    entity.animation.frameCount = 5;
    entity.animation.currentFrame = 0;
    entity.animation.frameWidth = 24;
    entity.animation.frameHeight = 24;
    entity.animation.frameAdvancement = 8;
    entity.speed = 80.0f;
    entity.gridPosition = gridPosition;
    entity.targetGridPosition = entity.gridPosition;
    entity.position = WorldPositionFromGridPosition(entity.gridPosition);
    entity.targetPosition = entity.position;
    entity.startPosition = entity.position;
    gameState.entities[handle.index] = entity;
}

B32 IsGridMovePossible(const GameState& gameState, Entity& entity, IVector2 targetGridPosition)
{
    B32 result { false };

    if (gameState.tileMap.tiles[targetGridPosition.y][targetGridPosition.x] != 1)
    {
        result = true;
    }

    for (const auto& e : gameState.entities)
    {
        if (e.kind == EntityKind::None) continue;
        if (e.kind == EntityKind::Effect) continue;
        if (e.kind == EntityKind::Enemy) continue;
        if (e.handle.id == entity.handle.id) continue;

        // NOTE:: Check against both current and target grid positions to account for entities mid-move
        if ((e.gridPosition.x == targetGridPosition.x && e.gridPosition.y == targetGridPosition.y) ||
            (e.targetGridPosition.x == targetGridPosition.x && e.targetGridPosition.y == targetGridPosition.y))
        {
            result = false;
            break;
        }
    }

    return result;
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

                // NOTE:: Only update animation row for entities with directional sprite sheets
                if (entity->kind == EntityKind::Player)
                {
                    switch (newDirection)
                    {
                    case Direction::Down:  entity->animation.row = 0; break;
                    case Direction::Up:    entity->animation.row = 1; break;
                    case Direction::Left:  entity->animation.row = 2; break;
                    case Direction::Right: entity->animation.row = 3; break;
                    default: break;
                    }
                }
            }

            IVector2 targetGridPosition { entity->gridPosition.x + gridMove.x, entity->gridPosition.y + gridMove.y };
            if (IsGridMovePossible(gameState, *entity, targetGridPosition))
            {
                entity->startPosition = entity->position;
                entity->targetGridPosition = targetGridPosition;
                entity->targetPosition = WorldPositionFromGridPosition(entity->targetGridPosition);
                entity->positionT = 0.0f;
            }
            // NOTE:: The input is consumed even if the grid move is blocked
            result = true;
        }
    }
    return result;
}

void UpdateEntityMovement(Entity& entity, F32 dt)
{

    F32 distance { Vector2Length(entity.targetPosition - entity.startPosition) };
    if (distance <= 0.0f)
    {
        return;
    }
    entity.positionT += (entity.speed * dt) / distance;
    if (entity.positionT >= 0.5f)
    {
        entity.gridPosition = entity.targetGridPosition;
    }
    if (entity.positionT >= 1.0f)
    {
        entity.positionT = 1.0f;
        entity.position = entity.targetPosition;
        entity.gridPosition = entity.targetGridPosition;
    }
    else
    {
        entity.position = Vector2Lerp(entity.startPosition, entity.targetPosition, entity.positionT);
    }
}

void UpdateAnimation(GameState& gameState, EntityHandle entityHandle, B32 playAnimation)
{
    Entity* entity { EntityFromHandle(gameState, entityHandle) };
    if (entity)
    {
        if (playAnimation)
        {
            if (gameState.tick % entity->animation.frameAdvancement == 0)
            {
                entity->animation.currentFrame = (entity->animation.currentFrame + 1) % entity->animation.frameCount;
            }
        }
        else
        {
            entity->animation.currentFrame = 0;
        }
    }
}

void DrawEntity(const Entity& entity)
{
    const SpriteAnimation& animation { entity.animation };

    Rectangle source
    {
        static_cast<F32>(animation.padding + animation.currentFrame * (animation.frameWidth + animation.padding)),
        static_cast<F32>(animation.padding + animation.row * (animation.frameHeight + animation.padding)),
        static_cast<F32>(animation.frameWidth),
        static_cast<F32>(animation.frameHeight)
    };

    DrawTextureRec(entity.texture, source, entity.position, WHITE);
}

void DrawGame(GameState& gameState)
{
    DrawTileMap(gameState.tileMap);

    // Draw lives
    {
        U32 xOffset { g_TileSize - 2 };
        U32 yOffset { g_TileSize };
        U32 padding { 2 };
        Texture2D lifeSprite { Assets::GetSpriteSheet(SpriteId::Life) };
        U32 stepX { lifeSprite.width + padding };
        for (int i { 0 }; i < gameState.lives; i++)
        {
            DrawTexture(lifeSprite, xOffset + i * stepX, yOffset, WHITE);
        }
    }

    for (const auto& entity : gameState.entities)
    {
        if (entity.kind == EntityKind::None) continue;
        if (entity.kind == EntityKind::Player) continue;
        DrawEntity(entity);
    }

    Entity* player { EntityFromHandle(gameState, gameState.playerHandle) };
    if (player)
    {
        const SpriteAnimation& animation { player->animation };

        Rectangle source
        {
            static_cast<F32>(animation.padding + animation.currentFrame * (animation.frameWidth + animation.padding)),
            static_cast<F32>(animation.padding + animation.row * (animation.frameHeight + animation.padding)),
            static_cast<F32>(animation.frameWidth),
            static_cast<F32>(animation.frameHeight)
        };

        B32 isInvincible { gameState.invincibilityT > 0.0f && std::fmod(gameState.invincibilityT, 0.1f * 2.0f) >= 0.1f };

        if (isInvincible)
        {
            DrawTextureRec(player->texture, source, player->position, { 255, 255,255, 125 });
        }
        else
        {
            DrawTextureRec(player->texture, source, player->position, WHITE);
        }
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
        AddEnemy(gameState, { 7,11 });
        AddBlock(gameState, BlockKind::Default, { 7,9 });
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

            if (IsKeyPressed(KEY_SPACE))
            {
                Entity* player { EntityFromHandle(gameState, gameState.playerHandle) };
                IVector2 facingGridPosition { IVector2FromDirection(player->direction) };
                IVector2 targetGridPosition { player->gridPosition.x + facingGridPosition.x, player->gridPosition.y + facingGridPosition.y };
                Entity* target { EntityFromGridPosition(gameState, EntityKind::Block, targetGridPosition) };
                if (target)
                {
                    IVector2 blockTargetGridPosition { target->gridPosition.x + facingGridPosition.x, target->gridPosition.y + facingGridPosition.y };
                    if (IsGridMovePossible(gameState, *target, blockTargetGridPosition))
                    {
                        target->pushed = true;
                        StartMove(gameState, target->handle, player->direction, facingGridPosition);
                    }
                    else
                    {
                        // Block gets crushed
                        if (target->blockKind == BlockKind::Default)
                        {
                            AddEffect(gameState, SpriteId::BlockCrushEffect, target->position, 4, 4);
                            target->kind = EntityKind::None;
                        }
                        else if (target->blockKind == BlockKind::FrozenEnemy)
                        {
                            AddEffect(gameState, SpriteId::JellyDeathEffect, target->position, 3, 8);
                            target->kind = EntityKind::None;
                        }
                    }
                }
            }

            for (auto& entity : gameState.entities)
            {
                if (entity.kind != EntityKind::Block) continue;
                if (entity.pushed)
                {
                    UpdateEntityMovement(entity, dt);
                    // NOTE:: when a movement step is complete start a new one if possible
                    if (entity.positionT >= 1.0f)
                    {
                        IVector2 nextMove { IVector2FromDirection(entity.direction) };
                        IVector2 nextGridPosition { entity.gridPosition.x + nextMove.x, entity.gridPosition.y + nextMove.y };
                        if (IsGridMovePossible(gameState, entity, nextGridPosition))
                        {
                            StartMove(gameState, entity.handle, entity.direction, nextMove);
                        }
                        else
                        {
                            entity.pushed = false;
                        }
                        
                    }
                }
            }

            Entity* player { EntityFromHandle(gameState, gameState.playerHandle) };
            UpdateEntityMovement(*player, dt);
            B32 isMoving { player && player->position != player->targetPosition };
            B32 queuedMovement { gameState.tapBuffer != Direction::None || hold != Direction::None };
            B32 isAnimating { isMoving || queuedMovement };
            UpdateAnimation(gameState, gameState.playerHandle, isAnimating);

            if (player)
            {
                Rectangle playerBounds { player->position.x, player->position.y, g_TileSize, g_TileSize };
                for (const auto& enemy : gameState.entities)
                {
                    if (enemy.kind == EntityKind::Enemy)
                    {
                        Rectangle enemyBounds { enemy.position.x, enemy.position.y, g_TileSize, g_TileSize };
                        if (CheckCollisionRecs(playerBounds, enemyBounds))
                        {
                            if (gameState.invincibilityT <= 0.0f)
                            {
                                gameState.lives--;
                                gameState.lives = std::clamp(gameState.lives, 0, 3);
                                gameState.invincibilityT = 1.5f;
                            }
                        }
                    }
                }
            }

            if (gameState.invincibilityT > 0.0f)
            {
                gameState.invincibilityT -= dt;
                if (gameState.invincibilityT < 0.0f)
                {
                    gameState.invincibilityT = 0.0f;
                }
            }

            for (auto& iceBlock : gameState.entities)
            {
                if (iceBlock.kind != EntityKind::Block) continue;
                if (iceBlock.blockKind != BlockKind::Ice) continue;
                if (!iceBlock.pushed) continue;

                Rectangle iceBlockBounds { iceBlock.position.x, iceBlock.position.y, g_TileSize, g_TileSize };

                for (auto& enemy : gameState.entities)
                {
                    if (enemy.kind != EntityKind::Enemy) continue;
                    Rectangle enemyBounds { enemy.position.x, enemy.position.y, g_TileSize, g_TileSize };
                    if (CheckCollisionRecs(iceBlockBounds, enemyBounds))
                    {
                        AddBlock(gameState, BlockKind::FrozenEnemy, enemy.gridPosition);
                        enemy.kind = EntityKind::None;
                        iceBlock.kind = EntityKind::None;
                        break;
                    }
                }
            }

            for (auto& block : gameState.entities)
            {
                if (block.kind != EntityKind::Block) continue;
                if (block.blockKind == BlockKind::FrozenEnemy) continue;
                if (!block.pushed) continue;

                Rectangle blockBounds { block.position.x, block.position.y, g_TileSize, g_TileSize };
                for (auto& enemy : gameState.entities)
                {
                    if (enemy.kind != EntityKind::Enemy) continue;
                    Rectangle enemyBounds { enemy.position.x, enemy.position.y, g_TileSize, g_TileSize };
                    if (CheckCollisionRecs(blockBounds, enemyBounds))
                    {
                        AddEffect(gameState, SpriteId::JellyDeathEffect, enemy.position, 3, 8);
                        enemy.kind = EntityKind::None;
                    }
                }
            }

            for (auto& entity : gameState.entities)
            {
                if (entity.kind == EntityKind::Effect)
                {
                    if (gameState.tick % entity.animation.frameAdvancement == 0)
                    {
                        entity.animation.currentFrame++;
                        // Despawn after playing all frames once
                        if (entity.animation.currentFrame >= entity.animation.frameCount)
                        {
                            entity.kind = EntityKind::None; // Free the slot
                        }
                    }
                }
            }

            for (auto& entity : gameState.entities)
            {
                if (entity.blockKind == BlockKind::FrozenEnemy || entity.kind == EntityKind::Enemy)
                {
                    UpdateAnimation(gameState, entity.handle, true);
                }
            }
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
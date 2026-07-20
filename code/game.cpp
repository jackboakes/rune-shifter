#include "game.h"

#include <algorithm>
#include <cassert>


Vector2 WorldPositionFromGridPosition(IVector2 gridPosition)
{
    return { static_cast<F32>(gridPosition.x) * static_cast<F32>(g_TileSize), static_cast<F32>(gridPosition.y) * static_cast<F32>(g_TileSize) };
}

void DrawTileMap(const TileMap& tileMap)
{
    for (int column { 0 }; column < tileMap.count.y; column++)
    {
        for (int row { 0 }; row < tileMap.count.x; row++)
        {
            IVector2 gridPosition { column, row };
            Vector2 position { WorldPositionFromGridPosition(gridPosition) };
            if (g_GameState.tileMap.tiles[column][row] == 0)
            {
                continue;
            }
            else if(g_GameState.tileMap.tiles[column][row] == 1)
            {
                DrawRectangle(position.x, position.y, tileMap.tileSize, tileMap.tileSize, WHITE);
            }
            else if (g_GameState.tileMap.tiles[column][row] == 2)
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

Entity* EntityFromHandle(EntityHandle handle)
{
    Entity* entity { &g_GameState.entities[handle.index] };
    if (entity->handle.id == handle.id)
    {
        return entity;
    }

    return nullptr;
}

void AddPlayer(GameState& g_GameState)
{
    Entity player;
    player.direction = Direction::Down;
    
    player.handle.id = g_GameState.nextEntityId;
    player.handle.index = 1;
    player.gridPosition = { 7,7 };
    player.position = WorldPositionFromGridPosition(player.gridPosition);

    g_GameState.entities[g_GameState.nextEntityId++] = player;

    g_GameState.playerHandle = player.handle;
}

void MovePlayer(Entity& player)
{
    if (IsKeyPressed(KEY_W))
    {
        player.gridPosition.y -= 1;
    }

    if (IsKeyPressed(KEY_A))
    {
        player.gridPosition.x -= 1;
    }

    if (IsKeyPressed(KEY_S))
    {
        player.gridPosition.y += 1;
    }

    if (IsKeyPressed(KEY_D))
    {
        player.gridPosition.x += 1;
    }

    player.position = WorldPositionFromGridPosition(player.gridPosition);
}

void DrawPlayer(Entity& player)
{
    DrawRectangle(player.position.x, player.position.y, g_TileSize, g_TileSize, YELLOW);
}

void DrawGame()
{
    DrawTileMap(g_GameState.tileMap);
    Entity* player { EntityFromHandle(g_GameState.playerHandle) };
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
    void Init()
    {
        g_GameState.tileMap.count = { g_TileMapCountX, g_TileMapCountY };
        g_GameState.tileMap.tileSize = g_TileSize;
        g_GameState.tileMap.tiles = tiles;

        AddPlayer(g_GameState);
    }
    
    static void Update(F32 dt)
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
            Entity* player { EntityFromHandle(g_GameState.playerHandle) };
            if (player)
            {
                MovePlayer(*player);
            }
        }
        break;
        }
    }

    static void DrawFrame()
    {
        BeginTextureMode(g_Target);
            ClearBackground(BLACK);

            switch (g_ProgramMode)
            {
            case ProgramMode::Game:
            {
                DrawGame();
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

    void UpdateAndDrawFrame(F32 dt)
    {
        Update(dt);
        DrawFrame();
    }
}
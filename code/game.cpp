#include "game.h"

#include <algorithm>


void DrawTileMap(const TileMap& tileMap)
{
    for (int column { 0 }; column < tileMap.count.y; column++)
    {
        for (int row { 0 }; row < tileMap.count.x; row++)
        {
            if (g_GameState.tileMap.tiles[column][row] == 0)
            {
                continue;
            }
            else if(g_GameState.tileMap.tiles[column][row] == 1)
            {
                DrawRectangle(column * 24, row * 24, 24, 24, WHITE);
            }
            else if (g_GameState.tileMap.tiles[column][row] == 2)
            {
                DrawRectangle(column * 24, row * 24, 24, 24, BLUE);
                DrawRectangleLines(column * 24, row * 24, 24, 24, BLACK);
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

void DrawGame()
{
    DrawTileMap(g_GameState.tileMap);
}

namespace Game
{
    void Init()
    {
        g_GameState.tileMap.count = { g_TileMapCountX, g_TileMapCountY };
        g_GameState.tileMap.tileSize = g_TileSize;
        g_GameState.tileMap.tiles = tiles;
    }
    
    static void Update(F32 dt)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ToggleMenu();
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
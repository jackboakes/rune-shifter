#include "raylib.h"

#include "game.h"

int main()
{
    // Disable raylib trace log messages
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(g_ScreenWidth, g_ScreenHeight, "Rune Shifter");
    SetWindowMinSize(720, 720);

    InitAudioDevice();

    GameState gameState;
    Game::Initialise(gameState);

    SetExitKey(KEY_NULL);

    g_Target = LoadRenderTexture(g_RenderTextureWidth, g_RenderTextureHeight);
    SetTextureFilter(g_Target.texture, TEXTURE_FILTER_POINT);

    SetTargetFPS(60);
    while (1)
    {
        if (WindowShouldClose()) break;
        gameState.tick++;
        F32 dt { GetFrameTime() };
        Game::UpdateAndDrawFrame(gameState, dt);
    }

    UnloadRenderTexture(g_Target);
    CloseWindow();
}
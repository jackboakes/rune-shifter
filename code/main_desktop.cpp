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

    Game::Init();

    SetExitKey(KEY_NULL);

    ///InitAudioDevice();

    g_Target = LoadRenderTexture(g_RenderTextureWidth, g_RenderTextureHeight);
    SetTextureFilter(g_Target.texture, TEXTURE_FILTER_POINT);

    SetTargetFPS(60);
    while (1)
    {
        if (WindowShouldClose()) break;
        F32 dt { GetFrameTime() };
        Game::UpdateAndDrawFrame(dt);
    }

    UnloadRenderTexture(g_Target);
    CloseWindow();
}
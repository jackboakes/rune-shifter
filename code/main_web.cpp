#include "raylib.h"
#include <emscripten/emscripten.h>

#include "game.h"

static GameState g_GameState;

static void MainLoop()
{
    g_GameState.tick++;
    F32 dt { GetFrameTime() };
    Game::UpdateAndDrawFrame(g_GameState, dt);
}

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

    Game::Initialise(g_GameState);

    SetExitKey(KEY_NULL);

    g_Target = LoadRenderTexture(g_RenderTextureWidth, g_RenderTextureHeight);
    SetTextureFilter(g_Target.texture, TEXTURE_FILTER_POINT);

    emscripten_set_main_loop(MainLoop, 60, 1);

    UnloadRenderTexture(g_Target);
    Game::Unload();
    CloseAudioDevice();
    CloseWindow();
}
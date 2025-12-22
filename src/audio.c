#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "audio.h"
#include <stdio.h>

static ma_engine g_engine;
static ma_sound g_bgm;
static bool g_engineInitialized = false;
static bool g_bgmLoaded = false;

bool initAudio(void) {
    if (g_engineInitialized) {
        return true;
    }

    ma_result result = ma_engine_init(NULL, &g_engine);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to initialize audio engine: %d\n", result);
        return false;
    }

    g_engineInitialized = true;
    return true;
}

bool playBGM(const char* filepath) {
    if (!g_engineInitialized) {
        fprintf(stderr, "Audio engine not initialized\n");
        return false;
    }

    // Stop any existing BGM
    if (g_bgmLoaded) {
        stopBGM();
    }

    ma_result result = ma_sound_init_from_file(&g_engine, filepath, 
        MA_SOUND_FLAG_STREAM, NULL, NULL, &g_bgm);
    
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to load BGM file '%s': %d\n", filepath, result);
        return false;
    }

    g_bgmLoaded = true;

    // Set looping
    ma_sound_set_looping(&g_bgm, MA_TRUE);

    // Start playback
    result = ma_sound_start(&g_bgm);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to start BGM playback: %d\n", result);
        return false;
    }

    return true;
}

void stopBGM(void) {
    if (g_bgmLoaded) {
        ma_sound_stop(&g_bgm);
        ma_sound_uninit(&g_bgm);
        g_bgmLoaded = false;
    }
}

void cleanupAudio(void) {
    stopBGM();
    
    if (g_engineInitialized) {
        ma_engine_uninit(&g_engine);
        g_engineInitialized = false;
    }
}

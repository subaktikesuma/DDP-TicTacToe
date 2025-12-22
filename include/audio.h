#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

/**
 * Initialize the audio engine
 * @return true on success, false on failure
 */
bool initAudio(void);

/**
 * Play background music from file (loops continuously)
 * @param filepath Path to the audio file (MP3, WAV, etc.)
 * @return true on success, false on failure
 */
bool playBGM(const char* filepath);

/**
 * Stop currently playing background music
 */
void stopBGM(void);

/**
 * Cleanup and uninitialize the audio engine
 */
void cleanupAudio(void);

#endif

#include <stdlib.h>
#include <time.h>

#include "ui.h"
#include "audio.h"

int main(void)
{
    srand(time(NULL));

    // Initialize audio and play background music
    if (initAudio()) {
        playBGM("sounds/bgm.mp3");
    }

    mainMenu();

    // Cleanup audio before exit
    cleanupAudio();

    return 0;
}
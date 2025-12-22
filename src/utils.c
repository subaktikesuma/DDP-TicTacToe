#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Include appropriate header for sleep function
#ifdef _WIN32
#include <windows.h>
// Define a cross-platform sleep function for milliseconds
void sleep_ms(int milliseconds) {
    Sleep(milliseconds);
}
#else
#define _DEFAULT_SOURCE
#include <unistd.h>
// Define a cross-platform sleep function for milliseconds (POSIX usleep takes microseconds)
void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}
#endif

/**
 * Fetches input from stdin
 */
char *input(char *msg, size_t size) {
    char *buffer = (char *)malloc(size);

    if (buffer == NULL) {
        free(buffer);
        return NULL;
    }

    printf("%s", msg);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

/**
 * Clears the current console stream
 */
void clear_console() {
    printf("\033[2J");

#ifdef _WIN32
    system("cls"); // NOLINT
#else
    system("clear"); // NOLINT
#endif
}

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        // Supporting larger values for n would requires an even more
        // elaborate implementation that combines multiple calls to rand()
        assert(n <= RAND_MAX);

        // Chop off all of the values that would cause skew...
        int end = RAND_MAX / n; // truncate skew
        assert(end > 0);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}

void slow_print(const char *str, int delay) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
        fflush(stdout); // Ensure the character is printed immediately
        sleep_ms(delay); // Delay between characters
    }
}
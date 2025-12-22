#ifndef BOT_H
#define BOT_H

typedef enum {
    EASY,
    MEDIUM,
    HARD,
} Difficulty;

void makeAIMove(char board[], char *aiPos, Difficulty difficulty);

#endif
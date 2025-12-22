#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

#define BOARD_SIZE 3

void initBoard(char board[]);

bool place(char board[], char sign, char signPos[]);
bool check(char board[], char sign);
bool checkTie(char board[]);

#endif
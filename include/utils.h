#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

char* input(char *msg, size_t size);
void clear_console();

int randint(int n);

void slow_print(const char *str, int delay);

#endif
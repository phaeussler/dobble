#pragma once
#include "logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX_CONNECTIONS;
int MAX_TIME_WAIT_CONNECTIONS;

unsigned char *cards;
int cards_defined;
char **words;
int *size;
char *obj_word;
int rounds;
int games;
int end_games;

void set_words();

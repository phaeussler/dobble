#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int MAX_CONNECTIONS;
int MAX_TIME_WAIT_CONNECTIONS;

unsigned char* cards;
int cards_defined;
char** words;
int* size;
char* obj_word;
int rounds;
int games;

void set_words();
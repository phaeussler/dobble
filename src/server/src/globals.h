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

void set_words();
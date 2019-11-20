#include "globals.h"


int MAX_CONNECTIONS = 10;
int MAX_TIME_WAIT_CONNECTIONS = 3; /* En segundos */


unsigned char* cards;
int cards_defined = 0;
char** words;
int* size;
char* obj_word;
int rounds;
int games = 1;
int end_games = 0;


void set_words(){
//   char* objective_word[50];
  words = calloc(1001, sizeof(char*));
  obj_word = calloc(21, 1);
  for(int i = 0; i < 1001; i++){
    words[i] = calloc(21, sizeof(char));
  }
  size = calloc(1001, sizeof(int));
  FILE* test = fopen("palabras.txt", "r");
  int cant = 0;
//   int aux_size;
  char aux_word[100];
  while(cant < 1001 && fscanf(test, "%d,%s", &size[cant], aux_word) != EOF)
  {
    strcpy(words[cant], aux_word);
    cant++;
  }
  fclose(test);
}

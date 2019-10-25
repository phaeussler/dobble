#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunication.h"
#include "conection.h"

char * revert(char * message){
  int len = strlen(message) + 1;
  char * response = malloc(len);
  
  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  printf("Iniciando Servidor\n");
  char* objective_word[50];
  char** list = calloc(1001, sizeof(char*));
  for(int i = 0; i < 1001; i++){
    list[i] = calloc(21, sizeof(char));
  }
  int* size = calloc(1001, sizeof(int));
  FILE* test = fopen("palabras.txt", "r");
  int cant = 0;
  int aux_size;
  char aux_word[100];
  while(cant < 1001 && fscanf(test, "%d,%s", &size[cant], aux_word) != EOF)
  {
    strcpy(list[cant], aux_word);
    cant++;
  }
  char * IP = "127.0.0.1";
  int PORT = 3000;

  unsigned char* cards;
  int cards_defined = 0;


  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Le enviamos al primer cliente un mensaje de bienvenida
  
  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->players[0]->sockets, players_info->players[1]->sockets};
  int my_attention = 0;
  while (1)
  {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);

    if (msg_code == 1)
    {
      server_connection_established(sockets_array[my_attention]);
    }
    else if (msg_code == 2){
      if(!cards_defined)
      {
        cards = send_words(list, size);
        cards_defined = 1;  
      }
      send(sockets_array[my_attention], cards, 2+cards[1], 0);
    }
    else if(msg_code == 20)
    {
      if(!cards_defined)
      {
        cards = send_words(list, size);
        cards_defined = 1;  
      }
      send(sockets_array[my_attention], cards, 2+cards[1], 0);

    }
    printf("------------------\n");
    my_attention = (my_attention + 1) % 2;
  }
  destroy_players_info(players_info);

  return 0;
}

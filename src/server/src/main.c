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


  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Le enviamos al primer cliente un mensaje de bienvenida
  char * welcome = "Bienvenido Cliente 1!!";
  server_send_message(players_info->sockets[0], 1, welcome);
  
  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->sockets[0], players_info->sockets[1]};
  int my_attention = 0;
  while (1)
  {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      printf("El cliente %d dice: %s\n", my_attention+1, client_message);
      
      // Le enviaremos el mismo mensaje invertido jeje
      char * response = revert(client_message);
      
      // Le enviamos la respuesta
      server_send_message(sockets_array[my_attention], 1, response);
    }
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      printf("Servidor traspasando desde %d a %d el mensaje: %s\n", my_attention+1, ((my_attention+1)%2)+1, client_message);
      
      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;
      
      server_send_message(sockets_array[my_attention], 2, client_message); 
    }
    else if(msg_code == 3)
    {

      send_words(sockets_array[my_attention], list, size);

      my_attention = (my_attention + 1) % 2;
    }
    printf("------------------\n");
  }
  destroy_players_info(players_info);

  return 0;
}

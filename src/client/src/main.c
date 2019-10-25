#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

int server_socket;

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}

int connect_to_server(char* IP, int PORT)
{
  char doing[10];
  printf("1: Connect\n");
  printf("2: Close\n");
  printf("What do you want to do: ");
  scanf("%s", doing);
  printf("------------------------");
  if(!strcmp(doing, "1"))
  {
    server_socket = prepare_socket(IP, PORT);
    client_start_conection(server_socket);
    return 1;
  }
  else if(!strcmp(doing, "2"))
  {
    return 0;
  }
  else
  {
    return connect_to_server(IP, PORT);
  }
  
  
}


int main (int argc, char *argv[]){
  printf("Iniciando Cliente\n");
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "127.0.0.1";
  int PORT = 3000;

  // Se prepara el socket
  

  connect_to_server(IP, PORT);

  while (1){
    int msg_code = client_receive_id(server_socket);
    
    if (msg_code == 2) {
 
      printf("Conction established\n");

      client_request_cards(server_socket);
    }

    else if (msg_code == 9)
    {
      print_cards(server_socket);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}

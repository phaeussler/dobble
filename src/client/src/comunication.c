#include "comunication.h"

int client_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

unsigned char * client_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  unsigned char * payload = calloc(len, 1);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

void client_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo. 
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.
  
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}

void client_start_conection(int client_socket)
{
  
  char msg[2];
  msg[0] = 1;
  // Se envía el paquete
  send(client_socket, msg, 1, 0);
}

void client_request_cards(int client_socket)
{
  
  char msg[2];
  msg[0] = 20;
  // Se envía el paquete
  send(client_socket, msg, 1, 0);
}

void print_one_word(char* word, int size, int pos)
{
    int printed = 0;
    while(printed < 20)
    {
        if(pos == printed)
        {
            for(int i = 0; i < size; i++)
            {
                printf("%c ", word[i]);
            }
            printed += size;
        }
        else
        {
            printf("- ");
            printed++;
        }
        
    }
}

void print_cards(int server_socket)
{
  unsigned char* payload = client_receive_payload(server_socket);

  char** first_card = calloc(10, sizeof(char*));
  for(int i = 0; i < 10; i++)
  {
    first_card[i] = calloc(20, sizeof(char));
  }
  
  char** second_card = calloc(10, sizeof(char*));
  for(int i = 0; i < 10; i++)
  {
    second_card[i] = calloc(20, sizeof(char));
  }

  int size1[10];
  int size2[10];
  int start1[10];
  int start2[10];

  int curr = 0;
  for(int i = 0; i < 10; i++)
  {
    size1[i] = (int)payload[curr];
    for(int j = 0; j < size1[i]; j++)
    {
      first_card[i][j] = payload[curr + 1 + j];
    }
    start1[i] = (int)payload[curr + size1[i] + 1];
    curr += size1[i] + 2;
  }

  for(int i = 0; i < 10; i++)
  {
    size2[i] = (int)payload[curr];
    for(int j = 0; j < size2[i]; j++)
    {
      second_card[i][j] = payload[curr + 1 + j];
    }
    start2[i] = (int)payload[curr + size2[i] + 1];
    curr += size2[i] + 2;
  }

  printf("              C A R T A 1                |                C A R T A 2              \n \n");
  for(int i = 0; i < 10; i++)
  {
    print_one_word(first_card[i], size1[i], start1[i]);
    printf(" |  ");
    print_one_word(second_card[i], size2[i], start2[i]);
    printf("\n");
    if (i < 9) printf("- - - - - - - - - - - - - - - - - - - -  |  - - - - - - - - - - - - - - - - - - - -\n");
  }

  free(payload);
  for(int i = 0; i < 10; i++)
  {
    free(first_card[i]);
    free(second_card[i]);
  }
  free(first_card);
  free(second_card);
}
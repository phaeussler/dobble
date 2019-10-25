#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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


int load_input(uint8_t *log, int *port, char **ip, int argc, char *argv[]){
  char *help_mg = " ./server -i <ip_address> -p <tcp-port> [-l]";  // Correct Usage String
  char *ip_flag =                                         "-i";  // Flag for ip address
  char *pt_flag =                                         "-p";  // flag for port
  char *lg_flag =                                         "-l";  // flag for log
  int ip_founded =                                           0;  // to check if ip is founded
  int pt_founded =                                           0;  // to check if port is founded
  int lg_founded =                                           0;  // to check if log is founded

  if (argc==6 || argc==5){
    for(int x=0; x < argc; x++){
      if(!strcmp(lg_flag, argv[x]) && !lg_founded){
        lg_founded = 1;
        *log = 1;
      }

      if(!strcmp(ip_flag, argv[x]) && !ip_founded){
        ip_founded = 1;
        x++;
        int addr_len = strlen(argv[x]);
        if(addr_len < 7 || 15 < addr_len){
          printf("\nERROR [%s]: Worng ip adress<%s>\n", __func__, argv[x]);
          return -1;
        }
        *ip = argv[x];
      }

      if(!strcmp(pt_flag, argv[x]) && !pt_founded){
        pt_founded = 1;
        x++;
        int port_entered = atoi(argv[x]);
        if(port_entered<0 || 65536<port_entered){
          printf("\nERROR [%s]: Worng port number<%s>\n", __func__, argv[x]);
          return -2;
        }
      *port = port_entered;
      }
    }

    if(!pt_founded || !ip_founded){
      printf("\nERROR [%s]: Missing important parameters(IP adresss and/or PORT)\n Correct usage: $%s\n", __func__, help_mg);
      return -3;
    }

    if(!lg_founded && argc==6){
      printf("\nERROR [%s]: Missing -l flag\n Correct usage: $%s\n", __func__, help_mg);
      return -4;
    }
    return 0;
  }

  printf("\nERROR [%s]: Wrong number of arguments <%d>\n Correct usage: $%s\n", __func__, argc, help_mg);
  return -5;
}

int main(int argc, char *argv[]){
  int debug = 0;
  char *IP;
  int PORT;
  uint8_t LOGG = 0;  // Si es 0, entonces no se hace logging, en otro caso si
  // Se define una IP y un puerto
  if(debug){
    IP = "127.0.0.1";
    PORT = 3000;
  }
  else{
    if(load_input(&LOGG, &PORT, &IP, argc, argv)){return -1;}
  }
  
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


  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Le enviamos al primer cliente un mensaje de bienvenida
  char * welcome = "Bienvenido Cliente 1!!";
  server_send_message(players_info->socket_c1, 1, welcome);
  
  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
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

  return 0;
}

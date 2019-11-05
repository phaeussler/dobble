#include "conection.h"
#include "globals.h"
#include <time.h>

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port){
  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar
  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in* client_addr[MAX_CONNECTIONS];
  socklen_t addr_size = sizeof(client_addr[0]);
  // struct sockaddr_in client1_addr;
  // struct sockaddr_in client2_addr;
  // socklen_t addr_size = sizeof(client1_addr);

  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  PlayersInfo * clients = malloc(sizeof(PlayersInfo));
  clients -> players = malloc(MAX_CONNECTIONS* sizeof(Player));
  clients -> connected = 0;

  // clock_t time_ready = 0;
  int rv;
  fd_set set;
  FD_ZERO(&set); /* clear the set */
  FD_SET(server_socket, &set); /* add our file descriptor to the set */

  int flags, err;

  struct timeval timeout;
  timeout.tv_usec = 0;
  // Se aceptan a los primeros 2 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación
  for (int client = 0; client < MAX_CONNECTIONS; client++)
  {
    if (client <= 1){
      clients -> players[client] = malloc(sizeof(Player));
      clients -> players[client] -> score = 0;
      clients -> players[client] -> nickname = NULL;
      clients -> players[client] -> win = 0;
      clients -> players[client] -> remaining = 3;
      clients -> players[client] -> waiting = 0;
      clients -> players[client] -> socket = accept(server_socket, (struct sockaddr *)&client_addr[client], &addr_size);
      clients -> players[client] -> last_code = 0;
      clients -> players[client] -> new_game = 0; 
      flags = fcntl(clients->players[client]->socket, F_GETFL, 0);
      err = fcntl(clients->players[client]->socket, F_SETFL, flags | O_NONBLOCK);
      clients -> connected ++;
      timeout.tv_sec = MAX_TIME_WAIT_CONNECTIONS;
    }
    else{
      rv = select(server_socket+1, &set, NULL, NULL, &timeout);
      if(rv == -1){
        perror("select"); /* an error accured */
        return clients;
      }
      else if(rv == 0){
        printf("Starting the game: timeout occurred (%d second) \n", MAX_TIME_WAIT_CONNECTIONS); /* a timeout occured */
        return clients;
      }
      else
      {
        // client_socket_fd = accept (server_socket,(struct sockaddr *) &client_name, &client_name_len);
        clients -> players[client] = malloc(sizeof(Player));
        clients -> players[client] -> score = 0;
        clients -> players[client] -> nickname = NULL;
        clients -> players[client] -> win = 0;
        clients -> players[client] -> remaining = 3;
        clients -> players[client] -> waiting = 0;
        clients -> players[client] -> socket = accept(server_socket, (struct sockaddr *)&client_addr[client], &addr_size);
        clients -> players[client] -> last_code = 0;
        clients -> players[client] -> new_game = 0; 
        flags = fcntl(clients->players[client]->socket, F_GETFL, 0);
        err = fcntl(clients->players[client]->socket, F_SETFL, flags | O_NONBLOCK);
        clients -> connected ++;
      }
    }
    printf("The client %d is ready\n", client);
  }
  return clients;
}


void destroy_players_info(PlayersInfo* players_info){
  for (int client = 0; client < players_info -> connected; client++)
  {
    if(players_info->players[client]->nickname) free(players_info->players[client]->nickname);
    close(players_info->players[client]->socket);
    free(players_info -> players[client]);
  }
  free(players_info -> players);
  free(players_info);
}
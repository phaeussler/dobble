#pragma once
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>


typedef struct player{
  int score;
  int socket;
  int win;
  char* nickname;
  int remaining;
  int waiting;
  int last_code;
} Player;

typedef struct players_info{
  Player** players;
  int connected;
  // int* theads;
  // int socket_c1;
  // int socket_c2;
} PlayersInfo;

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port);
void destroy_players_info(PlayersInfo* players_info);
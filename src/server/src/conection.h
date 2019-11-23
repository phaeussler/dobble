#pragma once
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include "logs.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct player
{
  int score;
  int socket;
  int win;
  char *nickname;
  int remaining;
  int waiting;
  int last_code;
  int new_game;
} Player;

typedef struct players_info
{
  Player **players;
  int connected;
  // int* theads;
  // int socket_c1;
  // int socket_c2;
} PlayersInfo;

PlayersInfo *prepare_sockets_and_get_clients(char *IP, int port);
void destroy_players_info(PlayersInfo *players_info);

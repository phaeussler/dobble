#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/socket.h>
#include "conection.h"
#pragma once

int server_receive_id(int client_socket);
char * server_receive_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char * message);
unsigned char* send_words(char** words, int* size);
void server_connection_established(int client_socket);
void server_ask_nikname(int client_socket);
void server_save_nickname(Player* player);
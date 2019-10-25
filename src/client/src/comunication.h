#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void client_send_message(int client_socket, int pkg_id, char * message);
int client_receive_id(int client_socket);
unsigned char * client_receive_payload(int client_socket);
void client_start_conection(int client_socket);
void print_cards(int server_socket);
void client_request_cards(int client_socket);
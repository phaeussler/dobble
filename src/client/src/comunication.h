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
void client_send_nickname(int server_socket, char* nickname);
void print_scores(int client_socket);
int client_recieve_myid(int client_socket);
int client_payload_len(int client_socket);
void client_send_obj_word(int server_socket, char* word);
void recieve_correct_answer(int client_socket);
void client_recieve_round_winners(int client_socket, int myId);
void client_recive_end_game(int client_socket);
void client_recive_game_winner(int client_socket, int myId);
void client_response_new_game(int server_socket, int answer);
void client_get_game(int server_socket);
void print_oponent_found(int server_socket);
int game_turn(int server_socket);

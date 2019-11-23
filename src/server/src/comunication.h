#pragma once
#include "conection.h"
#include "logs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <math.h>
#include "functions.h"
#include <time.h>

int server_receive_id(int client_socket);
char *server_receive_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char *message);
unsigned char *send_words(char **words, int *size, char *obj_word);
void server_connection_established(int client_socket);
void server_send_all_image(PlayersInfo* players, int* winners, int n_winners);
void server_ask_nikname(Player *player);
void server_bad_package(Player *player);
void server_save_nickname(Player *player);
void server_oponent_found(PlayersInfo *players_info);
void server_start_game(PlayersInfo *players_info, int game);
void server_send_ids(PlayersInfo *players_info);
void server_send_scores(PlayersInfo *players_info);
char *server_get_answer(Player *player);
void server_send_response_word(Player *players_info, int correct, int times);
void server_send_round_winner(PlayersInfo *players, int *winners, int n_winners);
void server_send_end_game(PlayersInfo *players, int games);
void server_send_game_winner(PlayersInfo *players, int *winners, int n_winners);
void server_ask_new_game(PlayersInfo *players);
void server_get_new_game_response(Player *player);
void server_send_disconect(PlayersInfo *players);
int server_payload_len(int client_socket);

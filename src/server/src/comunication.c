#include "comunication.h"

int server_receive_id(int client_socket) // Con Log
{
    infolog("Se obtiene solamente el ID del mensaje");
    int id = 0;
    int byte_recv = recv(client_socket, &id, 1, 0);
    if (byte_recv != 1)
    {
        char msg[255];
        sprintf(msg, "%s::Unable to recieve ID. Bytes espected: 1; Bytes IN: %d", __func__, byte_recv);
        error(msg);
        free(msg);
        return 0;
    }
    char msg[255];
    sprintf(msg, "%s::Received ID. Bytes IN: 1", __func__);
    infolog(msg);
    free(msg);
    return id;
}

int server_payload_len(int client_socket) // Con Log
{
    infolog("Se obtiene solamente el Tamaño(Size) del paquete");
    int len = 0;
    int byte_recv = recv(client_socket, &len, 1, 0);
    if (byte_recv != 1)
    {
        char msg[255];
        sprintf(msg, "%s::Unable to recieve Size. Bytes espected: 1; Bytes IN: %d", __func__, byte_recv);
        error(msg);
        free(msg);
        return 0;
    }
    char msg[255];
    sprintf(msg, "%s::Received Size. Bytes IN: 1", __func__);
    infolog(msg);
    free(msg);
    return len;
}

char *server_receive_payload(int client_socket) // Con Log
{
    infolog("Se obtiene el largo(Size) del payload");
    int len = 0;
    int byte_recv = recv(client_socket, &len, 1, 0);
    if (byte_recv != 1)
    {
        char msg[255];
        sprintf(msg, "%s::Unable to recieve Size. Bytes espected: 1; Bytes IN: %d", __func__, byte_recv);
        error(msg);
        free(msg);
        return NULL;
    }
    char msg[255];
    sprintf(msg, "%s::Received Size. Bytes IN: 1", __func__);
    infolog(msg);
    free(msg);
    char *payload = malloc(len);
    byte_recv = recv(client_socket, payload, len, 0);
    if (byte_recv != len)
    {
        char msg[255];
        sprintf(msg, "%s::Unable to recieve Payload. Bytes espected: %d; Bytes IN: %d", __func__, len, byte_recv);
        error(msg);
        free(msg);
        return NULL;
    }
    char msg[255];
    sprintf(msg, "%s::Received Payload. Bytes IN: %d", __func__, byte_recv);
    infolog(msg);
    free(msg);
    // Se retorna
    return payload;
}

void server_send_message(int client_socket, int pkg_id, char *message) // Con Log
{
    int payloadSize = strlen(message) + 1;
    //printf("payload size: %d\n", payloadSize);
    // Se arma el paquete
    char msg[1 + 1 + payloadSize];
    msg[0] = pkg_id;
    msg[1] = payloadSize;
    memcpy(&msg[2], message, payloadSize);
    // Se envía el paquete
    send(client_socket, msg, 2 + payloadSize, 0);
    char msg0[255 + 6 * payloadSize];
    sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
    for (int i = 0; i < payloadSize; i++)
        sprintf(msg0, "%#x ", msg[2 + i]);
    sprintf(msg0, "}");
    infolog(msg0);
    free(msg0);
}

int not_in(uint16_t *n_words, int word, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (n_words[i] == word)
            return 0;
    }
    return 1;
}
void random_numbers(uint16_t *n_words)
{
    int cant = 0;
    int zeros = 0;
    srand(time(NULL));
    while (cant < 19)
    {
        uint16_t new_n = rand() % 1001;
        if (!new_n)
        {
            if (!zeros)
            {
                n_words[cant] = new_n;
                zeros++;
                cant++;
            }
        }
        else if (not_in(n_words, new_n, 19))
        {
            n_words[cant] = new_n;
            cant++;
        }
    }
}

uint16_t *sep_words(uint16_t *n_words, int rep)
{
    uint16_t *new_list = calloc(9, sizeof(uint16_t));
    int cant = 0;
    int zeros = 0;
    while (cant < 9)
    {
        int new_n = rand() % 18;
        if (!n_words[new_n] && new_n != rep)
        {
            if (!zeros)
            {
                new_list[cant] = n_words[new_n];
                zeros++;
                cant++;
            }
        }
        else if (not_in(new_list, n_words[new_n], 9) && new_n != rep)
        {
            new_list[cant] = n_words[new_n];
            cant++;
        }
        // printf("cant: %d, new_n: %d\n", cant, new_n);
    }
    return new_list;
}

uint16_t *second_sep_words(uint16_t *n_words, uint16_t *sep_word, int rep)
{
    uint16_t *new_list = calloc(10, sizeof(uint16_t));
    int cant = 0;
    for (int i = 0; i < 19; i++)
    {
        if (not_in(sep_word, n_words[i], 9))
        {
            new_list[cant] = n_words[i];
            cant++;
        }
    }
    return new_list;
}

int add_words(uint16_t *card, unsigned char *payload, int *size, char **words, int cant, int curr, uint16_t *n_words, int rep)
{
    int pos_rep1 = -1;
    int pos;
    if (cant == 9)
        pos_rep1 = rand() % 8;
    for (int i = 0; i < 10; i++)
    {
        int mySize;
        char *word;
        if (cant == 10)
        {
            mySize = size[card[i]];
            word = words[card[i]];
        }
        else
        {
            if (i < pos_rep1)
            {
                mySize = size[card[i]];
                word = words[card[i]];
            }
            else if (i == pos_rep1)
            {
                mySize = size[n_words[rep]];
                word = words[n_words[rep]];
            }
            else
            {
                mySize = size[card[i - 1]];
                word = words[card[i - 1]];
            }
        }
        pos = rand() % (19 - mySize);

        // payload[curr] = mySize;
        memcpy(&payload[curr], &mySize, 1);
        for (int j = 0; j < mySize; j++)
        {
            // payload[curr + j + 1] = word[j];
            memcpy(&payload[curr + j + 1], &word[j], 1);
        }
        // payload[curr + mySize + 1] = pos;
        memcpy(&payload[curr + mySize + 1], &pos, 1);
        curr += mySize + 2;
    }

    return curr;
}

unsigned char *send_words(char **words, int *size, char *obj_word)
{

    uint16_t *n_words = calloc(19, sizeof(uint16_t));
    int rep = rand() % 18;

    random_numbers(n_words);
    strcpy(obj_word, words[n_words[rep]]);

    uint16_t *first_card = sep_words(n_words, rep);
    uint16_t *second_card = second_sep_words(n_words, first_card, rep);
    uint8_t payloadSize = 0;
    // printf("              C A R T A 1                |                C A R T A 2              \n \n");
    for (int i = 0; i < 10; i++)
    {
        payloadSize += size[second_card[i]];
        if (i < 9)
            payloadSize += size[first_card[i]];
        else
            payloadSize += size[n_words[rep]];
    }
    payloadSize += 40;
    unsigned char *sender = calloc(payloadSize + 1 + 1, sizeof(char));
    sender[0] = 9;
    sender[1] = payloadSize;
    int cant = 2;
    cant = add_words(first_card, sender, size, words, 9, cant, n_words, rep);
    add_words(second_card, sender, size, words, 10, cant, n_words, rep);
    free(first_card);
    free(second_card);
    // free(sender);
    free(n_words);
    return sender;
}

void server_connection_established(int client_socket) // Con Log
{
    char msg[2];
    msg[0] = 2;
    msg[1] = 0;
    // Se envía el paquete
    send(client_socket, msg, 2, 0);
    char msg0[255 + 6 * payloadSize];
    sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
    for (int i = 0; i < payloadSize; i++)
        sprintf(msg0, "%#x ", msg[2 + i]);
    sprintf(msg0, "}");
    infolog(msg0);
    free(msg0);
}

void server_ask_nikname(Player *player) // Con Log
{
    char msg[2];
    msg[0] = 3;
    msg[1] = 0;
    // Se envía el paquete
    send(player->socket, msg, 2, 0);
    char msg0[255 + 6 * payloadSize];
    sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
    for (int i = 0; i < payloadSize; i++)
        sprintf(msg0, "%#x ", msg[2 + i]);
    sprintf(msg0, "}");
    infolog(msg0);
    free(msg0);
}

void server_bad_package(Player *player) // Con Log
{
    char msg[2];
    msg[0] = 20;
    msg[1] = 0;
    // Se envía el paquete
    send(player->socket, msg, 2, 0);
    char msg0[255 + 6 * masg[1]];
    sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
    for (int i = 0; i < msg[1]; i++)
        sprintf(msg0, "%#x ", msg[2 + i]);
    sprintf(msg0, "}");
    infolog(msg0);
    free(msg0);
    free(msg); // Segun yo no hace mal ;)
}

void server_save_nickname(Player *player) // Con Log
{
    int len = 0;
    recv(player->socket, &len, 1, 0);
    player->nickname = malloc(len);
    int byte_recv = recv(player->socket, player->nickname, len, 0);
    if (byte_recv != len)
    {
        char msg[255];
        sprintf(msg, "%s::Unable to recieve Payload. Bytes espected: %d; Bytes IN: %d", __func__, len, byte_recv);
        error(msg);
        free(msg);
        server_bad_package(player);
        return;
    }
    char msg0[255 + 6 * len];
    sprintf(msg0, "%s::Recieved Package %s. Bytes IN: {ID=4}{Size=%d}{Payload=%s}", __func__, MSG_NAMES[4], len, player->nickname);
    infolog(msg0);
    free(msg0);
    player->waiting = 1;
}

void server_oponent_found(PlayersInfo *players_info) // Con Log
{
    int max_len = 0;
    for (int i = 0; i < players_info->connected; i++)
    {
        max_len += strlen(players_info->players[i]->nickname);
    }
    char msg[max_len + 3 + (3 * players_info->connected)];
    msg[0] = 5;
    char *nick;
    // Se envía el paquete
    int curr = 2;
    int to_send = 0;
    char jump = 10;
    char line = 45;
    char space = 32;
    for (int x = 0; x < players_info->connected; x++)
    {
        for (int i = 0; i < players_info->connected; i++)
        {
            if (i != x)
            {
                nick = players_info->players[i]->nickname;
                to_send += strlen(nick) + 3;
                memcpy(&msg[curr], &jump, 1);
                memcpy(&msg[curr + 1], &line, 1);
                memcpy(&msg[curr + 2], &space, 1);
                curr += 3;
                for (int j = 0; j < strlen(nick); j++)
                {
                    memcpy(&msg[curr], &nick[j], 1);
                    curr++;
                }
            }
        }
        msg[curr] = '\0';
        msg[1] = to_send + 1;
        send(players_info->players[x]->socket, msg, to_send + 3, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=%s}", __func__, MSG_NAMES[msg[0]], msg[0], msg[1], &msg[2]);
        infolog(msg0);
        free(msg0);
        curr = 2;
        to_send = 0;
    }
}

void server_start_game(PlayersInfo *players_info, int game) // Con Log
{
    char msg[3];
    msg[0] = 7;
    msg[1] = 1;
    msg[2] = game;
    // Se envía el paquete
    for (int i = 0; i < players_info->connected; i++)
    {
        send(players_info->players[i]->socket, msg, 3, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=%d}", __func__, MSG_NAMES[msg[0]], msg[0], msg[1], msg[2]);
        infolog(msg0);
        free(msg0);
    }
}

void server_send_ids(PlayersInfo *players_info) // Con Log
{

    for (int i = 0; i < players_info->connected; i++)
    {
        unsigned char msg[3];
        msg[0] = 6;
        msg[1] = 1;
        msg[2] = i + 1;
        send(players_info->players[i]->socket, msg, 3, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=%d}", __func__, MSG_NAMES[msg[0]], msg[0], msg[1], msg[2]);
        infolog(msg0);
        free(msg0);
    }
}

void server_send_scores(PlayersInfo *players_info) // Con Log
{
    int n_players = players_info->connected;
    char *sender = calloc(1 + 1 + n_players, 1);
    sender[0] = 8;
    sender[1] = n_players;
    int curr = 2;
    for (int i = 0; i < n_players; i++)
    {
        sender[curr] = players_info->players[i]->win;
        curr++;
        for (int j = 0; j < n_players; j++)
        {
            if (i != j)
            {
                sender[curr] = players_info->players[j]->win;
                curr++;
            }
        }
        send(players_info->players[i]->socket, sender, 2 + n_players, 0);
        char msg0[255 + 6 * sender[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[sender[0]], sender[0], sender[1]);
        for (int i = 0; i < sender[1]; i++)
            sprintf(msg0, "%#x ", sender[2 + i]);
        sprintf(msg0, "}");
        infolog(msg0);
        free(msg0);
        curr = 2;
    }
    free(sender);
}

char *server_get_answer(Player *player) // TODO
{
    char *ans = calloc(21, 1);
    int len = 0;
    recv(player->socket, &len, 1, 0);
    int byte_recv = recv(player->socket, ans, len, 0);
    if (byte_recv != len)
    {
        /* TODO: ID not recieved properly */
        return -1;
    }
    return ans;
}

void server_send_response_word(Player *players_info, int correct, int times) // Con Log
{
    char msg[4];
    msg[0] = 11;
    msg[1] = 2;
    msg[2] = correct;
    msg[3] = times;
    // Se envía el paquete
    send(players_info->socket, msg, 4, 0);
    char msg0[255 + 6 * msg[1]];
    sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
    for (int i = 0; i < msg[1]; i++)
        sprintf(msg0, "%#x ", msg[2 + i]);
    sprintf(msg0, "}");
    infolog(msg0);
    free(msg0);
}

void server_send_round_winner(PlayersInfo *players, int *winners, int n_winners) // Con Log
{
    unsigned char *msg;
    int senders = n_winners;
    if (players->connected > n_winners)
    {
        msg = calloc(2 + n_winners, 1);
        msg[0] = 12;
        msg[1] = n_winners;
        for (int j = 0; j < n_winners; j++)
        {
            msg[2 + j] = winners[j] + 1;
        }
    }
    else
    {
        msg = calloc(3, 1);
        msg[0] = 12;
        msg[1] = 1;
        msg[2] = 0;
        senders = 1;
    }

    for (int i = 0; i < players->connected; i++)
    {
        send(players->players[i]->socket, msg, 2 + senders, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=%d} ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1], msg[2]);
        infolog(msg0);
        free(msg0);
    }
    free(msg);
}

void server_send_end_game(PlayersInfo *players, int games) // Con Log
{
    unsigned char *msg = calloc(3, 1);
    msg[0] = 13;
    msg[1] = 1;
    msg[2] = games;
    for (int i = 0; i < players->connected; i++)
    {
        send(players->players[i]->socket, msg, 3, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=%d}", __func__, MSG_NAMES[msg[0]], msg[0], msg[1], msg[2]);
        infolog(msg0);
        free(msg0);
    }
    free(msg);
}

void server_send_game_winner(PlayersInfo *players, int *winners, int n_winners) // Con Log
{
    unsigned char *msg = calloc(2 + n_winners, 1);
    int senders = n_winners;

    msg[0] = 14;
    msg[1] = n_winners;
    for (int j = 0; j < n_winners; j++)
    {
        msg[2 + j] = winners[j] + 1;
    }

    for (int i = 0; i < players->connected; i++)
    {
        send(players->players[i]->socket, msg, 2 + senders, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
        for (int i = 0; i < msg[1]; i++)
            sprintf(msg0, "%#x ", msg[2 + i]);
        sprintf(msg0, "}");
        infolog(msg0);
        free(msg0);
    }
    free(msg);
}

void server_ask_new_game(PlayersInfo *players) // Con Log
{
    unsigned char *msg = calloc(2, 1);
    msg[0] = 15;
    msg[1] = 0;

    for (int i = 0; i < players->connected; i++)
    {
        send(players->players[i]->socket, msg, 2, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload=}", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
        infolog(msg0);
        free(msg0);
    }
    free(msg);
}

void server_get_new_game_response(Player *player) // TODO
{
    int len = 0;
    int answer = 0;
    recv(player->socket, &len, 1, 0);
    int byte_recv = recv(player->socket, &answer, len, 0);
    if (byte_recv != len + 1)
    {
        /* TODO: ID not recieved properly */
        return (void)NULL;
    }
    if (answer)
        player->new_game = 1;
    else
        player->new_game = -1;
}

void server_send_disconect(PlayersInfo *players) // Con Log
{
    unsigned char *msg = calloc(2, 1);
    msg[0] = 17;
    msg[1] = 0;
    for (int i = 0; i < players->connected; i++)
    {
        send(players->players[i]->socket, msg, 2, 0);
        char msg0[255 + 6 * msg[1]];
        sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
        for (int i = 0; i < msg[1]; i++)
            sprintf(msg0, "%#x ", msg[2 + i]);
        sprintf(msg0, "}");
        infolog(msg0);
        free(msg0);
        close(players->players[i]->socket);
    }
    free(msg);
}

int server_send_image(int client_socket, char * file_name){
    int payloadSize = 255;
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL){
        perror("File");
        return 2;
    }
    fseek(fp, 0L, SEEK_END);
    int totalBytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    int totalPayloads = ceil(totalBytes/payloadSize) + 1; // (1 byte): La cantidad total de Payloads que serán enviados.

    char msg[4+payloadSize];
    msg[0] = 64;
    msg[1] = totalPayloads;

    unsigned int bytes_readead;
    int currentPayload = 1; // (1 byte): N° del Payload que se está enviando actualmente (se parte desde 1).Payload Size (1 byte): Tamaño en bytes de la información de la imagen.
    while((bytes_readead = fread(&msg[4], 1, payloadSize, fp))>0 ){
        printf("[SERVER][PKGE OUT] sending image segment. TP: %d. CP: %d. PS: %d\n", totalPayloads, currentPayload, bytes_readead);
        msg[2] = currentPayload;
        msg[3] = bytes_readead;
        send(client_socket, msg, 4 + bytes_readead, 0);
        currentPayload ++;
        if (currentPayload > totalPayloads) break;
    }
    fclose(fp);
    return 0;
}

void server_send_all_image(PlayersInfo* players, int* winners, int n_winners){
    char* file_name;
    for (int i = 0; i < players->connected; i++)
    {
        if (valueinarray(i, winners, n_winners)){
           file_name = "assets/winner.png";
        }
        else{
           file_name = "assets/loser.png";
        }
        file_name = "example.txt";
        printf("Sending image %s to socket %d \n", file_name, i);
        server_send_image(players->players[i]->socket, file_name);
    }
}

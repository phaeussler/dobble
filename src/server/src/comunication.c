#include "comunication.h"

int server_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char * server_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  char * payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

void server_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1;
  //printf("payload size: %d\n", payloadSize);
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}

int not_in(uint16_t* n_words, int word, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(n_words[i] == word) return 0;
    }
    return 1;
}
void random_numbers(uint16_t* n_words)
{
    int cant = 0;
    int zeros = 0;
    srand(time(NULL));
    while(cant < 19)
    {
        uint16_t new_n = rand() % 1001;
        if(!new_n)
        {
            if(!zeros)
            {    
                n_words[cant] = new_n;
                zeros++;
                cant++;
            }
        }
        else if(not_in(n_words, new_n, 19))
        {
            n_words[cant] = new_n;
            cant++;
        }
    }
}

uint16_t* sep_words(uint16_t* n_words, int rep)
{
    uint16_t* new_list = calloc(9, sizeof(uint16_t));
    int cant = 0; 
    int zeros = 0;
    while(cant < 9)
    {
        int new_n = rand() % 18;
        if(!n_words[new_n] && new_n != rep)
        {
            if(!zeros)
            {    
                new_list[cant] = n_words[new_n];
                zeros++;
                cant++;
            }
        }
        else if(not_in(new_list, n_words[new_n], 9) && new_n != rep)
        {
            new_list[cant] = n_words[new_n];
            cant++;
        }
        // printf("cant: %d, new_n: %d\n", cant, new_n);
    }
    return new_list;
}

uint16_t* second_sep_words(uint16_t* n_words, uint16_t* sep_word, int rep)
{
    uint16_t* new_list = calloc(10, sizeof(uint16_t));
    int cant = 0;
    for(int i = 0; i < 19; i++){
        if(not_in(sep_word, n_words[i], 9))
        {
            new_list[cant] = n_words[i];
            cant++;
        }
    }
    return new_list;
}

int add_words(uint16_t* card, unsigned char* payload, int* size, char** words, int cant, int curr, uint16_t* n_words, int rep)
{
    int pos_rep1 = -1;
    int pos;
    if(cant == 9) pos_rep1 = rand() % 8;
    for(int i = 0; i < 10; i++)
    {
        int mySize;
        char* word;
        if(cant == 10)
        {
            mySize = size[card[i]];
            word = words[card[i]];
        }
        else
        {   
            if(i < pos_rep1)
            {
                mySize = size[card[i]];
                word = words[card[i]];
            }
            else if(i == pos_rep1)
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
        for(int j = 0; j < mySize; j++){
            payload[curr + j + 1] = word[j];
        }
        // payload[curr + mySize + 1] = pos;
        memcpy(&payload[curr + mySize + 1], &pos, 1);
        curr += mySize + 2;

    }

    return curr;
}

void send_words(int client_socket, char** words, int* size)
{

    uint16_t* n_words = calloc(19, sizeof(uint16_t));
    int rep = rand() % 18;

    random_numbers(n_words);
    
    uint16_t* first_card = sep_words(n_words, rep);
    uint16_t* second_card = second_sep_words(n_words, first_card, rep);
    uint8_t payloadSize = 0;
    // printf("              C A R T A 1                |                C A R T A 2              \n \n");
    for(int i = 0; i < 10; i++)
    {
        payloadSize += size[second_card[i]];
        if(i < 9) payloadSize += size[first_card[i]];
        else payloadSize += size[n_words[rep]];
    }
    payloadSize += 40;
    unsigned char* sender = calloc(payloadSize + 1 + 1, sizeof(char));
    sender[0] = 9;
    sender[1] = payloadSize;
    int cant = 2;
    cant = add_words(first_card, sender, size, words, 9, cant, n_words, rep);
    add_words(second_card, sender, size, words, 10, cant, n_words, rep);
    send(client_socket, sender, 2+payloadSize, 0);
    // int curr = 0;
    // printf("%d ", sender[0]);
    // printf("%d \n", sender[1]);
    // curr += 2;
    // for(int i = 0; i < 20; i++){
    //     int a = sender[curr];
    //     printf("%d ", a);
    //     for(int j = 0; j < a; j++){
    //         printf("%c", sender[curr+1+j]);
    //     }
    //     printf(" %d\n", sender[curr +a+1]);
    //     curr += a+2;
    // }
    free(first_card);
    free(second_card);
    free(sender);
    free(n_words);
}
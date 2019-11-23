#include "comunication.h"
#include "logs.h"

extern char **LAST_PACK_SENT;

int client_receive_id(int client_socket)
{
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received message ID=%d", __func__, id);
  infolog(msg);
  free(msg);
  return id;
}

int client_payload_len(int client_socket)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: 1", __func__);
  infolog(msg);
  free(msg);
  return len;
}

unsigned char *client_receive_payload(int client_socket)
{
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  unsigned char *payload = calloc(len, 1);
  int received = recv(client_socket, payload, len, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, received);
  infolog(msg);
  free(msg);
  return payload;
}

void client_send_message(int client_socket, int pkg_id, char *message)
{
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo.
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.

  // Se arma el paquete
  char msg[1 + 1 + payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2 + payloadSize, 0);
  char msg0[255 + 6 * msg[1]];
  sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
  for (int i = 0; i < msg[1]; i++)
    sprintf(msg0, "%#x ", msg[2 + i]);
  sprintf(msg0, "}");
  infolog(msg0);
  free(msg0);
  *LAST_PACK_SENT = &msg;
}

void client_start_conection(int client_socket)
{

  char msg[2];
  msg[0] = 1;
  msg[1] = 0;
  // Se envía el paquete
  send(client_socket, msg, 2, 0);
  char msg0[255 + 6 * msg[1]];
  sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
  infolog(msg0);
  *LAST_PACK_SENT = &msg;
}

void print_one_word(char *word, int size, int pos)
{
  int printed = 0;
  while (printed < 20)
  {
    if (pos == printed)
    {
      for (int i = 0; i < size; i++)
      {
        printf("%c ", word[i]);
      }
      printed += size;
    }
    else
    {
      printf("- ");
      printed++;
    }
  }
}

void print_cards(int server_socket)
{
  unsigned char *payload = client_receive_payload(server_socket);

  char **first_card = calloc(10, sizeof(char *));
  for (int i = 0; i < 10; i++)
  {
    first_card[i] = calloc(20, sizeof(char));
  }

  char **second_card = calloc(10, sizeof(char *));
  for (int i = 0; i < 10; i++)
  {
    second_card[i] = calloc(20, sizeof(char));
  }

  int size1[10];
  int size2[10];
  int start1[10];
  int start2[10];

  int curr = 0;
  for (int i = 0; i < 10; i++)
  {
    size1[i] = (int)payload[curr];
    for (int j = 0; j < size1[i]; j++)
    {
      first_card[i][j] = payload[curr + 1 + j];
    }
    start1[i] = (int)payload[curr + size1[i] + 1];
    curr += size1[i] + 2;
  }

  for (int i = 0; i < 10; i++)
  {
    size2[i] = (int)payload[curr];
    for (int j = 0; j < size2[i]; j++)
    {
      second_card[i][j] = payload[curr + 1 + j];
    }
    start2[i] = (int)payload[curr + size2[i] + 1];
    curr += size2[i] + 2;
  }

  printf("              C A R T A 1                |                C A R T A 2              \n \n");
  for (int i = 0; i < 10; i++)
  {
    print_one_word(first_card[i], size1[i], start1[i]);
    printf(" |  ");
    print_one_word(second_card[i], size2[i], start2[i]);
    printf("\n");
    if (i < 9)
      printf("- - - - - - - - - - - - - - - - - - - -  |  - - - - - - - - - - - - - - - - - - - -\n");
  }

  free(payload);
  for (int i = 0; i < 10; i++)
  {
    free(first_card[i]);
    free(second_card[i]);
  }
  free(first_card);
  free(second_card);
}

void client_send_nickname(int server_socket, char *nickname)
{
  int payloadSize = strlen(nickname) + 1;

  char msg[1 + 1 + payloadSize];
  msg[0] = 4;
  msg[1] = payloadSize;
  memcpy(&msg[2], nickname, payloadSize);

  send(server_socket, msg, 2 + payloadSize, 0);
  char msg0[255 + 6 * msg[1]];
  sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
  for (int i = 0; i < msg[1]; i++)
    sprintf(msg0, "%#x ", msg[2 + i]);
  sprintf(msg0, "}");
  infolog(msg0);
  free(msg0);
  *LAST_PACK_SENT = &msg;
}

void print_scores(int client_socket)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  unsigned char *payload = calloc(len, 1);
  int received = recv(client_socket, payload, len, 0);
  char msg[255];
  sprintf(msg, "%s::Received Scores. Bytes IN: %d - %d", __func__, payload[0], payload[1]);
  infolog(msg);
  free(msg);
  printf("\n My score: %d", payload[0]);
  printf("\n Oponent(s) score: ");
  for (int i = 1; i < len; i++)
  {
    printf("\n - %d", payload[i]);
  }
  free(payload);
}

int client_recieve_myid(int client_socket)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  int id = 0;
  recv(client_socket, &id, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received client ID=%d", __func__, id);
  infolog(msg);
  free(msg);
  return id;
}

void client_send_obj_word(int server_socket, char *word)
{
  int payloadSize = strlen(word) + 1;

  char msg[1 + 1 + payloadSize];
  msg[0] = 10;
  msg[1] = payloadSize;
  memcpy(&msg[2], word, payloadSize);

  send(server_socket, msg, 2 + payloadSize, 0);
  char msg0[255 + 6 * msg[1]];
  sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
  for (int i = 0; i < msg[1]; i++)
    sprintf(msg0, "%#x ", msg[2 + i]);
  sprintf(msg0, "}");
  infolog(msg0);
  free(msg0);
  *LAST_PACK_SENT = &msg;
}

void recieve_correct_answer(int client_socket)
{
  int len = 0;
  int intentos = 0;
  int correct = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  recv(client_socket, &correct, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Correct. Bytes IN: %d", __func__, correct);
  infolog(msg);
  free(msg);
  recv(client_socket, &intentos, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Intentos. Bytes IN: %d", __func__, intentos);
  infolog(msg);
  free(msg);
  if (correct)
    printf("The answer is correct!!\n");
  else
    printf("Incorrect answer! You have %d more attempts\n", intentos);
}

void client_recieve_round_winners(int client_socket, int myId)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  unsigned int *payload = calloc(10, sizeof(unsigned int));
  for (int i = 0; i < len; i++)
  {
    recv(client_socket, &payload[i], 1, 0);
  }

  int first_val = 1;
  first_val = payload[0];
  if (len == 1 && !first_val)
    printf("All players tie\n");
  else
  {
    for (int i = 0; i < len; i++)
    {
      if (myId == (payload[i]))
        printf("Congratulations, you won this round\n");
    }
    for (int i = 0; i < len; i++)
    {
      if (myId != (payload[i]))
        printf("Player %d won this round\n", payload[i]);
    }
  }
  free(payload);
}

void client_recive_end_game(int client_socket)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received message size=%d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  int game = 0;
  recv(client_socket, &game, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received message Payload=%d", __func__, game);
  infolog(msg);
  free(msg);
  printf("Game n°%d has ended\n", game);
}

void client_recive_game_winner(int client_socket, int myId)
{
  int len = 0;
  recv(client_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  // Se obtiene el payload
  unsigned int *winners = calloc(len, sizeof(unsigned int));
  recv(client_socket, winners, len, 0);
  int i_winner = 0;
  for (int i = 0; i < len; i++)
  {
    if ((int)winners[i] == myId)
      i_winner++;
  }

  if (i_winner > 0)
    printf("Congratulations!! you won the game\n");
  else
    printf("You lost the game, better luck next time\n");

  free(winners);
}

void client_response_new_game(int server_socket, int answer)
{
  char msg[3];
  msg[0] = 16;
  msg[1] = 1;
  msg[2] = answer;

  send(server_socket, msg, 3, 0);
  char msg0[255 + 6 * msg[1]];
  sprintf(msg0, "%s::Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[msg[0]], msg[0], msg[1]);
  for (int i = 0; i < msg[1]; i++)
    sprintf(msg0, "%#x ", msg[2 + i]);
  sprintf(msg0, "}");
  infolog(msg0);
  free(msg0);
  *LAST_PACK_SENT = &msg;
}

void client_get_game(int server_socket)
{
  int len = 0;
  recv(server_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  int game_number = 0;
  recv(server_socket, &game_number, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Game number. Bytes IN: %d", __func__, game_number);
  infolog(msg);
  free(msg);
  printf("Starting game N°%d\n", game_number);
}

void print_oponent_found(int server_socket)
{
  int len = 0;
  recv(server_socket, &len, 1, 0);
  char msg[255];
  sprintf(msg, "%s::Received Size. Bytes IN: %d", __func__, len);
  infolog(msg);
  free(msg);
  char *oponents = calloc(len, 1);
  recv(server_socket, oponents, len, 0);
  char msg[255];
  sprintf(msg, "%s::Received Oponents. Bytes IN: %s", __func__, oponents);
  infolog(msg);
  free(msg);
  printf("Oponent(s) found:");
  for (int i = 0; i < len; i++)
  {
    printf("%c", oponents[i]);
  }

  free(oponents);
}



void client_send_disconect(int server_socket)
{
    // unsigned char* msg = calloc(2, 1);
    char msg[3];
    msg[0] = 17;
    msg[1] = 0;
    send(server_socket, msg, 2, 0);
    // free(msg);
}

int game_turn(int server_socket){
  char doing[10];
  char answer_word[21];
  printf("Menu:\n");
  printf("1: Send an answer\n");
  printf("2: Disconect\n");
  printf("What do you want to do: ");
  scanf("%s", doing);
  if(!strcmp(doing, "1"))
  {
    printf("\nWhich word is repeated?: \n");
    scanf("%s", answer_word);
    client_send_obj_word(server_socket, answer_word);
    return 1;
  }
  else if(!strcmp(doing, "2"))
  {
    client_send_disconect(server_socket);
    return 0;
  }
  else
  {
    printf("Please select a valid option (1 or 2)\n");
    return game_turn(server_socket);
  }
}

int totalbytesreaded = 0;

void client_recive_image(int server_socket, char* buff){
    int totalPayloads;
    int currentPayload;
    unsigned int payloadSize;
    recv(server_socket, &totalPayloads, 1, 0);
    recv(server_socket, &currentPayload, 1, 0);
    recv(server_socket, &payloadSize, 1, 0);
    if (currentPayload==1){
      printf("seting buff size %d\n", totalPayloads * payloadSize);
    //   buff = calloc(totalPayloads * payloadSize, sizeof(char));
    }

    char mini_buff[payloadSize];
    printf("[CLIENT][PKGE IN] sending image segment. TP: %d. CP: %d. PS: %d\n", totalPayloads, currentPayload, payloadSize);
    int b = recv(server_socket, &mini_buff, payloadSize, 0);
    printf("b %d totalbytesreaded %d \n", b, totalbytesreaded);
    totalbytesreaded += b;
    printf("buff:\n%s\n\n\n", mini_buff);
    // if (currentPayload == totalPayloads){
        FILE* fp = fopen( "image.txt", "ab");
        if(fp != NULL){
            fwrite(mini_buff, 1, payloadSize, fp);
            if (b<0){
                perror("Receiving");
            }
        fclose(fp);
        }
        else {
            perror("File");
        }
        printf("End recibing\n");
     }

 
void client_resend(int server_socket)
{
  send(server_socket, LAST_PACK_SENT, LAST_PACK_SENT[1]);
  char msg0[255 + 6 * LAST_PACK_SENT[1]];
  sprintf(msg0, "%s::Re-Sent Package %s. Bytes OUT: {ID=%d}{Size=%d}{Payload= ", __func__, MSG_NAMES[LAST_PACK_SENT[0]], LAST_PACK_SENT[0], LAST_PACK_SENT[1]);
  for (int i = 0; i < LAST_PACK_SENT[1]; i++)
    sprintf(msg0, "%#x ", LAST_PACK_SENT[2 + i]);
  sprintf(msg0, "}");
  infolog(msg0);
  free(msg0);
}

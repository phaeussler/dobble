#include <unistd.h>
#include <stdio.h>
#include "logs.h"
#include "conection.h"
#include "comunication.h"

int server_socket;

char *get_input()
{
  char *response = malloc(20);
  int pos = 0;
  while (1)
  {
    char c = getchar();
    if (c == '\n')
      break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}

int get_new_game_response()
{
  char ans[50];
  while (1)
  {
    printf("Do you want to play a new game?\n");
    printf("1) Yes\n2) No\n");
    scanf("%s", ans);
    if (!strcmp(ans, "1") || !strcmp(ans, "2"))
      break;
    else
      printf("Wrong answer! Please try again\n");
  }
  int real_ans = atoi(ans);
  if (real_ans == 1)
    return 1;
  else
    return 0;
}

int connect_to_server(char *IP, int PORT)
{
  char doing[10];
  printf("1: Connect\n");
  printf("2: Close\n");
  printf("What do you want to do: ");
  scanf("%s", doing);
  printf("------------------------\n");
  if (!strcmp(doing, "1"))
  {
    server_socket = prepare_socket(IP, PORT);
    client_start_conection(server_socket);
    return 1;
  }
  else if (!strcmp(doing, "2"))
  {
    return 0;
  }
  else
  {
    return connect_to_server(IP, PORT);
  }
}

int load_input(uint8_t *log, int *port, char **ip, int argc, char *argv[])
{
  char *help_mg = " ./server -i <ip_address> -p <tcp-port> [-l]"; // Correct Usage String
  char *ip_flag = "-i";                                           // Flag for ip address
  char *pt_flag = "-p";                                           // flag for port
  char *lg_flag = "-l";                                           // flag for log
  int ip_founded = 0;                                             // to check if ip is founded
  int pt_founded = 0;                                             // to check if port is founded
  int lg_founded = 0;                                             // to check if log is founded

  if (argc == 6 || argc == 5)
  {
    for (int x = 0; x < argc; x++)
    {
      if (!strcmp(lg_flag, argv[x]) && !lg_founded)
      {
        lg_founded = 1;
        *log = 1;
      }

      if (!strcmp(ip_flag, argv[x]) && !ip_founded)
      {
        ip_founded = 1;
        x++;
        int addr_len = strlen(argv[x]);
        if (addr_len < 7 || 15 < addr_len)
        {
          printf("\nERROR [%s]: Worng ip adress<%s>\n", __func__, argv[x]);
          return -1;
        }
        *ip = argv[x];
      }

      if (!strcmp(pt_flag, argv[x]) && !pt_founded)
      {
        pt_founded = 1;
        x++;
        int port_entered = atoi(argv[x]);
        if (port_entered < 0 || 65536 < port_entered)
        {
          printf("\nERROR [%s]: Worng port number<%s>\n", __func__, argv[x]);
          return -2;
        }
        *port = port_entered;
      }
    }

    if (!pt_founded || !ip_founded)
    {
      printf("\nERROR [%s]: Missing important parameters(IP adresss and/or PORT)\n Correct usage: $%s\n", __func__, help_mg);
      return -3;
    }

    if (!lg_founded && argc == 6)
    {
      printf("\nERROR [%s]: Missing -l flag\n Correct usage: $%s\n", __func__, help_mg);
      return -4;
    }
    return 0;
  }

  printf("\nERROR [%s]: Wrong number of arguments <%d>\n Correct usage: $%s\n", __func__, argc, help_mg);
  return -5;
}

int main(int argc, char *argv[])
{
  char nickname[50];

  //char answer_word[21];
  DEBUGGER = 1;
  char *IP;
  int PORT;
  int myId = 0;
  uint8_t LOGG = 0; // Si es 0, entonces no se hace logging, en otro caso si
  // Se define una IP y un puerto
  if (DEBUGGER)
  {
    IP = "127.0.0.1";
    PORT = 3000;
  }
  else
  {
    if (load_input(&LOGG, &PORT, &IP, argc, argv))
    {
      return -1;
    }
  }
  setup_logger("log.txt", LOGG);
  debuger("Iniciando Cliente, llamando funcion: {connect_to_server(%s, %d)}", IP, PORT);
  // Se prepara el socket

  int start = connect_to_server(IP, PORT);

  int msg_code;
  while (start){
    msg_code = client_receive_id(server_socket);
    // printf("msg_code: %d \n", msg_code);
    
    if (msg_code == 2) {

      client_payload_len(server_socket);
      printf("Conection established\n");

      // client_request_cards(server_socket);
    }

    else if (msg_code == 3)
    {
      client_payload_len(server_socket);
      printf("\nType your Nickname: ");
      scanf("%s", nickname);
      client_send_nickname(server_socket, nickname);
    }

    else if (msg_code == 5)
    {
      print_oponent_found(server_socket);
    }

    else if (msg_code == 6)
    {
      myId = client_recieve_myid(server_socket);
      printf("\nMy id: %d\n", myId);
    }

    else if (msg_code == 7)
    {
      client_get_game(server_socket);
      /* Seteo el timer */
    }

    else if (msg_code == 9)
    {
      print_cards(server_socket);
      game_turn(server_socket);
      
      // printf("\nWhich word is repeated?: ");
      // scanf("%s", answer_word);
      // client_send_obj_word(server_socket, answer_word);
    }

    else if (msg_code == 8)
    {
      print_scores(server_socket);
    }

    else if (msg_code == 11)
    {
      recieve_correct_answer(server_socket);
    }

    else if (msg_code == 12)
    {
      client_recieve_round_winners(server_socket, myId);
    }

    else if (msg_code == 13)
    {
      client_recive_end_game(server_socket);
    }

    else if (msg_code == 14)
    {
      client_recive_game_winner(server_socket, myId);
    }

    else if (msg_code == 15)
    {
      int play_new_game = 0;
      play_new_game = get_new_game_response();
      client_response_new_game(server_socket, play_new_game);
    }

    else if (msg_code == 17)
    {
      client_payload_len(server_socket);
      printf("Disconected\n");
      break;
    }
    else if(msg_code == 64)
    {
      char* buff;
      client_recive_image(server_socket, buff);
    }
    
    else if (msg_code == 20)
    {
      client_resend();
    }
    // printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  // free(IP);

  return 0;
}

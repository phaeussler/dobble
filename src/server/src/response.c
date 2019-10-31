#include "response.h"


extern unsigned char* cards;
extern int cards_defined;
extern char** words;
extern int* size;


// void start_connection(){}


// void connection_established(PlayersInfo * players_info, int my_attention){
//     if(!cards_defined)
//     {
//         cards = send_words(words, size);
//         cards_defined = 1;  
//     }
//     send(players_info -> players[my_attention] -> socket, cards, 2 + cards[1], 0);
// }


// void ask_nickname(){}


// void return_nickname(){}


// void opponent_found(){}


// void send_ids(){}


// void start_game(){}


// void scores(){}


void server_send_cards(PlayersInfo * players_info){
  if(!cards_defined){
    cards = send_words(words, size);
    cards_defined = 1;  
  }
  for (int i = 0; i < players_info->connected; i++)
  {
    send(players_info -> players[i] -> socket, cards, 2 + cards[1], 0);
  }
}


// void response_word(){}


// void round_winner_loser(){}


// void end_game(){}


// void game_winner_loser(){}


// void ask_new_game(){}


// void answer_new_game(){}


// void disconnect(){}


// void error_bad_Package(){}


// void bonus_image(){}


void handle_message(PlayersInfo * players_info, int my_attention, int msg_code){
  if (msg_code){
    players_info -> players[my_attention] -> last_code = msg_code;
    printf("SET player: %d code: %d\n", my_attention, players_info->players[my_attention]->last_code);
  }
  /* Recibe el mensaje y busca la función correspondiente al mensaje */
  if (msg_code == 1){
  /* Start Connection */
    server_connection_established(players_info->players[my_attention]->socket);
    server_ask_nikname(players_info->players[my_attention]);
  }
  else if (msg_code == 2){
    /* Connection Established */
  
  }
  else if (msg_code == 3){
    /* Ask Nickname */

  }
  else if (msg_code == 4){
    /* Return Nickname */
    server_save_nickname(players_info->players[my_attention]);
    if(!waitting_clients(players_info)){
      printf("Send oponent found\n");
      server_oponent_found(players_info);
      server_start_game(players_info);
      /* SEND SCORE */
      server_send_cards(players_info);
    }
  }
  else if (msg_code == 5){
    /* Opponent Found */

  }
  else if (msg_code == 6){
    /* Send IDs */

  }
  else if (msg_code == 7){
    /* Start Game */

  }
  else if (msg_code == 8){
    /* Scores */

  }
  else if (msg_code == 9){
    /* Send Cards */
    // send_cards(players_info, my_attention, msg_code);
  }
  else if (msg_code == 10){
    /* Send Word */

  }
  else if (msg_code == 11){
    /* Response Word */

  }
  else if (msg_code == 12){
    /* Round Winner/Loser */

  }
  else if (msg_code == 13){
    /* End Game */

  }
  else if (msg_code == 14){
    /* Game Winner/Loser */

  }
  else if (msg_code == 15){
    /* Ask New Game */

  }
  else if (msg_code == 16){
    /* Answer New Game */

  }
  else if (msg_code == 17){
    /* Disconnect */

  }
  else if (msg_code == 20){
    /* Error Bad Package */
    // if(!cards_defined)
    //   {
    //     cards = send_words(words, size);
    //     cards_defined = 1;  
    //   }
    //   send(players_info->players[my_attention]->socket, cards, 2+cards[1], 0);
  }
  else if (msg_code == 64){
    /* Bonus Image */
  }
}

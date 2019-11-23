#include "response.h"

extern unsigned char *cards;
extern int cards_defined;
extern char **words;
extern int *size;
extern char *obj_word;
extern int rounds;

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

void server_send_cards(PlayersInfo *players_info)
{
  if (!cards_defined)
  {
    cards = send_words(words, size, obj_word);
    cards_defined = 1;
  }
  for (int i = 0; i < players_info->connected; i++)
  {
    send(players_info->players[i]->socket, cards, 2 + cards[1], 0);
  }
}

void server_send_cards_to_player(Player *player)
{
  send(player->socket, cards, 2 + cards[1], 0);
}

void str_to_uppercase(char *str)
{
  int j = 0;
  // char ch;
  while (str[j])
  {
    // ch = str[j];
    str[j] = toupper(str[j]);
    j++;
  }
}

int get_round_winner(PlayersInfo *players, int *winners)
{
  int n_winners = 0;
  int big = 0;
  for (int i = 0; i < players->connected; i++)
  {
    if (players->players[i]->score >= big)
      big = players->players[i]->score;
  }
  for (int i = 0; i < players->connected; i++)
  {
    if (players->players[i]->score >= big)
    {
      winners[n_winners] = i;
      n_winners++;
    }
  }
  return n_winners;
}

int get_game_winner(PlayersInfo *players, int *winners)
{
  int n_winners = 0;
  int big = 0;
  for (int i = 0; i < players->connected; i++)
  {
    if (players->players[i]->win >= big)
      big = players->players[i]->win;
  }
  for (int i = 0; i < players->connected; i++)
  {
    if (players->players[i]->win >= big)
    {
      winners[n_winners] = i;
      n_winners++;
    }
  }
  return n_winners;
}

int get_game_winner_by_disconnect(PlayersInfo* players, int* winners, int my_attention)
{
  int n_winners = 0;
  for(int i = 0; i < players->connected; i++)
  {
    printf("%d != %d => %d\n", i, my_attention, i != my_attention);
    if(i != my_attention)
    {
      winners[n_winners] = i;
      n_winners ++;
    }
  }
  return n_winners;
}


void reset_score_and_remaining(PlayersInfo* players)
{
  for (int i = 0; i < players->connected; i++)
  {
    players->players[i]->score = 0;
    players->players[i]->remaining = 3;
  }
}

void reset_wins(PlayersInfo *players)
{
  for (int i = 0; i < players->connected; i++)
  {
    players->players[i]->win = 0;
  }
}

// void response_word(){}

// void round_winner_loser(){}

// void end_game(){}

// void game_winner_loser(){}

// void ask_new_game(){}

// void answer_new_game(){}

// void disconnect(){}

void error_bad_Package_sent()
{
  /* TODO: LOG: Se ha enviado un paquete invalido o corrupto */
}

// void bonus_image(){}

void handle_message(PlayersInfo *players_info, int my_attention, int msg_code)
{

  /* Recibe el mensaje y busca la función correspondiente al mensaje */
  if (msg_code == 1)
  {
    /* Start Connection */
    server_payload_len(players_info->players[my_attention]->socket);
    server_connection_established(players_info->players[my_attention]->socket);
    server_ask_nikname(players_info->players[my_attention]);
  }
  else if (msg_code == 2)
  {
    /* Connection Established */
  }
  else if (msg_code == 3)
  {
    /* Ask Nickname */
  }
  else if (msg_code == 4)
  {
    /* Return Nickname */
    server_save_nickname(players_info->players[my_attention]);
    if (!waitting_clients(players_info))
    {
      printf("Send oponent found\n");
      server_oponent_found(players_info);
      server_send_ids(players_info);
      server_start_game(players_info, games);
      server_send_scores(players_info);
      server_send_cards(players_info);
      take_waitting(players_info);
    }
  }
  else if (msg_code == 5)
  {
    /* Opponent Found */
  }
  else if (msg_code == 6)
  {
    /* Send IDs */
  }
  else if (msg_code == 7)
  {
    /* Start Game */
  }
  else if (msg_code == 8)
  {
    /* Scores */
  }
  else if (msg_code == 9)
  {
    /* Send Cards */
    // send_cards(players_info, my_attention, msg_code);
  }
  else if (msg_code == 10)
  {
    int correct = 0;
    char *ans;
    int remainings = players_info->players[my_attention]->remaining;
    ans = server_get_answer(players_info->players[my_attention]);
    str_to_uppercase(ans);
    printf("obj: %s\n", obj_word);
    printf("answer: %s\n", ans);
    if (!strcmp(obj_word, ans))
      correct = 1;
    if (correct)
    {
      players_info->players[my_attention]->waiting = 1;
      players_info->players[my_attention]->score += 5 - (3 - remainings) * 2;
      players_info->players[my_attention]->win += 5 - (3 - remainings) * 2;
      server_send_response_word(players_info->players[my_attention], correct, remainings - 1);
    }
    else
    {
      if (players_info->players[my_attention]->remaining > 1)
      {
        players_info->players[my_attention]->remaining--;
        server_send_response_word(players_info->players[my_attention], correct, remainings - 1);
        server_send_cards_to_player(players_info->players[my_attention]);
      }
      else
      {
        players_info->players[my_attention]->waiting = 1;
        players_info->players[my_attention]->score += 0;
        players_info->players[my_attention]->win += 0;
        server_send_response_word(players_info->players[my_attention], correct, 0);
      }
    }
    if (!waitting_clients(players_info))
    {
      rounds += 1;
      int winners[10];
      int n_winners = 0;
      n_winners = get_round_winner(players_info, winners);
      server_send_round_winner(players_info, winners, n_winners);
      take_waitting(players_info);
      reset_score_and_remaining(players_info);
      cards_defined = 0;
      if (rounds < 5)
      {
        server_send_scores(players_info);
        server_send_cards(players_info);
      }
      else
      {
        int game_winners[10];
        int n_game_winners = 0;
        n_game_winners = get_game_winner(players_info, game_winners);
        server_send_end_game(players_info, games);
        server_send_game_winner(players_info, game_winners, n_game_winners);
        server_send_all_image(players_info, game_winners, n_game_winners);
        reset_wins(players_info);
        games++;
        server_ask_new_game(players_info);
      }
    }
    free(ans);
  }
  else if (msg_code == 11)
  {
    /* Response Word */
  }
  else if (msg_code == 12)
  {
    /* Round Winner/Loser */
  }
  else if (msg_code == 13)
  {
    /* End Game */
  }
  else if (msg_code == 14)
  {
    /* Game Winner/Loser */
  }
  else if (msg_code == 15)
  {
    /* Ask New Game */
  }
  else if (msg_code == 16)
  {
    /* Answer New Game */
    server_get_new_game_response(players_info->players[my_attention]);
    if (new_game_players(players_info) == 1)
    {
      rounds = 0;
      server_start_game(players_info, games);
      server_send_scores(players_info);
      server_send_cards(players_info);
    }
    else if (new_game_players(players_info) == -1)
    {
      server_send_disconect(players_info);
      end_games = 1;
    }
  }
  else if (msg_code == 17)
  {
    /* Disconnect */
    printf("A client is asking for disconect\n");
    int game_winners[10];
    int n_game_winners = 0;
    n_game_winners = get_game_winner_by_disconnect(players_info, game_winners, my_attention);
    for (int i = 0; i < n_game_winners; i++)
    {
      printf("ganador %d es %d\n", i, game_winners[i]);
    }
    
    server_send_game_winner(players_info, game_winners, n_game_winners);
    server_send_all_image(players_info, game_winners, n_game_winners);
    server_send_disconect(players_info);
    end_games = 1;
  }
  else if (msg_code == 20)
  {
    /* Error Bad Package: Reenviar estado de la partida a todos los jugadores */
    // if(!cards_defined)
    //   {
    //     cards = send_words(words, size);
    //     cards_defined = 1;
    //   }
    //   send(players_info->players[my_attention]->socket, cards, 2+cards[1], 0);
  }
  else if (msg_code == 64)
  {
    /* TODO: Bonus Image */
  }
  else
  {
    char *err = "Bad Package ERROR: Wrong ID";
    server_bad_package(players_info->players[my_attention], err);
  }
}

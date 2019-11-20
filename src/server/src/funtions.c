#include "functions.h"

int waitting_clients(PlayersInfo* players)
{
    /* Si no estoy esperando retorno 1 */
  for(int i = 0; i < players->connected; i++)
  {
    if(!players->players[i]->waiting){
      return 1;
    }
  }
  return 0;
}

void take_waitting(PlayersInfo* players)
{
  for(int i = 0; i < players->connected; i++)
  {
    players->players[i]->waiting = 0;
  }
}

int new_game_players(PlayersInfo* players)
{
  int responses = 0;
  for(int i = 0; i < players->connected; i++)
  {
    if(players->players[i]->new_game == 1) responses++;
    else if(players->players[i]->new_game == -1) return -1;
  }

  if(responses == players->connected) return 1;
  else return 0;
}

int valueinarray(int val, int* arr, int len)
{
  int i;
  for(i = 0; i < len; i++)
  {
    if(arr[i] == val)
      return 1;
  }
  return 0;
}

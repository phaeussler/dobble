#include "functions.h"

int waitting_clients(PlayersInfo* players)
{
    /* Si estoy esperando retorno 1 */
  for(int i = 0; i < players->connected; i++)
  {
    if(players->players[i]->waiting){
      printf("Waiting client: %d\n", i);
      return 1;
    }
  }
  return 0;
}

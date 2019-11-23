#pragma once
#include "conection.h"
#include "logs.h"

int waitting_clients(PlayersInfo *players);
void take_waitting(PlayersInfo *players);
int new_game_players(PlayersInfo *players);

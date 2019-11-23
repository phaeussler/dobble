#pragma once
#include "comunication.h"
#include "conection.h"
#include "functions.h"
#include "globals.h"
#include "logs.h"
#include <ctype.h>

void handle_message(PlayersInfo *players_info, int my_attention, int msg_code);

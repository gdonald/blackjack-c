#ifndef GAME_H
#define GAME_H

#define MAX_PLAYER_HANDS 7
#define MIN_BET 500
#define MAX_BET 10000000
#define SAVE_FILE "bj.txt"

#include <stdlib.h>
#include <time.h>

#include "buffer_toggle.h"
#include "shoe.h"
#include "dealer_hand.h"
#include "player_hand.h"

void deal_new_hand();

#endif

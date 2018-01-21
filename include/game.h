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

unsigned money;
unsigned current_bet;
unsigned total_player_hands;

void deal_new_hand();
void draw_hands();
void ask_insurance();
void pay_hands();
void bet_options();
void save_game();
void clear();

unsigned all_bets();

#endif

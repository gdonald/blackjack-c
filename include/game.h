#ifndef GAME_H
#define GAME_H

#define MAX_PLAYER_HANDS 7
#define MIN_BET 500
#define MAX_BET 10000000
#define SAVE_FILE "bj.txt"

#include <stdlib.h>
#include <time.h>

#include "buffer_toggle.h"
#include "dealer_hand.h"
#include "player_hand.h"
#include "shoe.h"

unsigned money;
unsigned current_bet;
unsigned total_player_hands;
unsigned current_player_hand;
unsigned num_decks;

Shoe shoe;

bool more_hands_to_play();
bool need_to_play_dealer_hand();

void deal_new_hand();
void draw_hands();
void ask_insurance();
void pay_hands();
void bet_options();
void save_game();
void clear();
void play_more_hands();
void play_dealer_hand();
void process();
void draw_dealer_hand();
void insure_hand();
void no_insurance();
void normalize_bet();
void get_new_bet();

unsigned all_bets();

#endif

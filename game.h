#ifndef GAME_H
#define GAME_H

#define MAX_PLAYER_HANDS 7

#include <stdlib.h>
#include <time.h>

#include "shoe.h"
#include "hand.h"

Shoe shoe;
Hand dealer_hand;
Hand player_hands[MAX_PLAYER_HANDS];

int run(Shoe *shoe, Hand *dealer_hand, Hand *player_hands);
void deal_new_hand(Shoe *shoe, Hand *dealer_hand, Hand *player_hands);

#endif

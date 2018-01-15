#ifndef GAME_H
#define GAME_H

#define MAX_PLAYER_HANDS 7

#include <stdlib.h>
#include <time.h>

#include "shoe.h"
#include "dealer_hand.h"
#include "player_hand.h"

Shoe shoe;
DealerHand dealer_hand;
PlayerHand player_hands[MAX_PLAYER_HANDS];

int run(Shoe *shoe, DealerHand *dealer_hand, PlayerHand *player_hands);
void deal_new_hand(Shoe *shoe, DealerHand *dealer_hand, PlayerHand *player_hands);

#endif

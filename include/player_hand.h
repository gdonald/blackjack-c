#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include <stdbool.h>

#include "hand.h"
#include "shoe.h"

typedef enum { Unknown, Won, Lost, Push } HandStatus;

typedef struct
{
  bool stood;
  bool played;
  Card cards[MAX_CARDS_PER_HAND];
  unsigned num_cards;
} PlayerHand;

bool player_is_busted(PlayerHand *hand);
bool player_is_blackjack(PlayerHand *hand);
bool player_is_done(PlayerHand *hand);
bool player_can_hit(PlayerHand *hand);

unsigned player_get_value(PlayerHand *hand, CountMethod method);

void player_deal_card(PlayerHand *hand, Shoe *shoe);
void player_draw(PlayerHand *hand);
void player_get_action(PlayerHand *hand);

#endif

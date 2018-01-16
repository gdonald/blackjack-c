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

bool ph_is_busted(PlayerHand *hand);
bool ph_is_blackjack(PlayerHand *hand);
bool ph_is_done(PlayerHand *hand);

unsigned ph_get_value(PlayerHand *hand, CountMethod method);

void ph_deal_card(PlayerHand *hand, Shoe *shoe);
void ph_draw(PlayerHand *hand);

#endif

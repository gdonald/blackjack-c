#ifndef DEALER_HAND_H
#define DEALER_HAND_H

#include <stdbool.h>

#include "card.h"
#include "game.h"
#include "hand.h"
#include "shoe.h"

typedef struct
{
  Card cards[MAX_CARDS_PER_HAND];
  unsigned num_cards;
  bool hide_down_card;
  bool played;
} DealerHand;

extern DealerHand dealer_hand;

void dealer_deal_card();

bool dealer_upcard_is_ace();
bool dealer_is_done();
bool dealer_is_blackjack();
bool dealer_is_busted();

unsigned dealer_get_value(CountMethod method);

#endif

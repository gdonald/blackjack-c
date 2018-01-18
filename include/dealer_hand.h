#ifndef DEALER_HAND_H
#define DEALER_HAND_H

#include <stdbool.h>

#include "hand.h"
#include "card.h"
#include "shoe.h"

typedef struct
{
  Card cards[MAX_CARDS_PER_HAND];
  unsigned num_cards;
  bool hide_down_card;
} DealerHand;

void dealer_deal_card(DealerHand *hand, Shoe *shoe);

bool dealer_upcard_is_ace(DealerHand *dealer_hand);
bool dealer_is_done(DealerHand *hand);

#endif

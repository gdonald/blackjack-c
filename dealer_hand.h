#ifndef DEALER_HAND_H
#define DEALER_HAND_H

#include <stdbool.h>

#include "hand.h"
#include "card.h"

typedef struct
{
  bool stood;
  bool played;
  Card cards[MAX_CARDS_PER_HAND];
  unsigned num_cards;
} DealerHand;


#endif

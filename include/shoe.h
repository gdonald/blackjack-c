#ifndef SHOE_H
#define SHOE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "card.h"

#define CARDS_PER_DECK 52
#define MAX_DECKS 8

const unsigned shuffle_specs[8][2];

typedef struct
{
  unsigned current_card;
  Card cards[CARDS_PER_DECK * MAX_DECKS];
} Shoe;
 
bool need_to_shuffle(Shoe *shoe, unsigned num_decks);
void shuffle(Shoe *shoe, unsigned num_decks);
void swap(Card *a, Card *b);
void new_regular(Shoe *shoe, unsigned num_decks);
Card deal_card(Shoe *shoe);

#endif

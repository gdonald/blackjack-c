#ifndef HAND_H
#define HAND_H

#define MAX_CARDS_PER_HAND 12

#include <stdbool.h>

#include "shoe.h"

typedef enum { Unknown, Won, Lost, Push } HandStatus;
typedef enum { Soft, Hard } CountMethod;

typedef struct
{
  bool stood;
  bool played;
  Card cards[MAX_CARDS_PER_HAND];
  unsigned num_cards;
} Hand;

bool is_busted(Hand *hand);
bool is_blackjack(Hand *hand);
bool is_done(Hand *hand);

unsigned get_value(Hand *hand, CountMethod method);

void deal_card(Hand *hand, Shoe *shoe);
void draw(Hand *hand);

#endif

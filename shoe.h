#ifndef SHOE_H
#define SHOE_H

#include <stdbool.h>

//#include "card.h"

struct shoe
{
  int number_decks;
  //Card cards[416];
};

typedef struct shoe Shoe;
  
bool need_to_shuffle();
void shuffle();
void new_regular();

#endif

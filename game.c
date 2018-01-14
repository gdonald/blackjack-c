#include "game.h"

void deal_new_hand()
{
  if(need_to_shuffle())
  {
    shuffle();
  }
}

int run()
{
  deal_new_hand();
  return 0;
}



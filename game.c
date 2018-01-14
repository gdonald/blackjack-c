#include "game.h"

void deal_new_hand(Shoe *shoe, Hand *dealer_hand, Hand *player_hands)
{
  if(need_to_shuffle(shoe))
  {
    shuffle(shoe);
  }

  Card c1 = {};
  dealer_hand->cards[0] = c1;
  Card c2 = {};
  dealer_hand->cards[1] = c2;

  Hand h;
  player_hands[0] = h;

}

int run(Shoe *shoe, Hand *dealer_hand, Hand *player_hands)
{

  deal_new_hand(shoe, dealer_hand, player_hands);

  return 0;
}



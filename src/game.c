#include "game.h"

unsigned num_decks = 1;
unsigned money = 10000;
unsigned current_bet = 500;

void deal_new_hand(Shoe *shoe, DealerHand *dealer_hand, PlayerHand *player_hands)
{
  if(need_to_shuffle(shoe))
  {
    shuffle(shoe);
  }

  Card c1 = {};
  dealer_hand->cards[0] = c1;
  Card c2 = {};
  dealer_hand->cards[1] = c2;

  PlayerHand ph;
  player_hands[0] = ph;

}

int run(Shoe *shoe, DealerHand *dealer_hand, PlayerHand *player_hands)
{

  deal_new_hand(shoe, dealer_hand, player_hands);

  return 0;
}



#include "game.h"

unsigned num_decks = 1;
unsigned money = 10000;
unsigned current_bet = 500;

Shoe shoe = {};
DealerHand dealer_hand;
PlayerHand player_hands[MAX_PLAYER_HANDS];

void deal_new_hand()
{
  if(need_to_shuffle(&shoe))
  {
    shuffle(&shoe);
  }

  Card c1 = {};
  dealer_hand.cards[0] = c1;
  Card c2 = {};
  dealer_hand.cards[1] = c2;

  PlayerHand ph;
  player_hands[0] = ph;
}

int main()
{
  srand(time(NULL));
  bt_off();
  deal_new_hand(&shoe, &dealer_hand, player_hands);
  bt_on();

  return 0;
}

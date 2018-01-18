#include "game.h"

unsigned num_decks = 1;
unsigned money = 10000;
unsigned current_bet = 500;
unsigned current_player_hand = 0;
unsigned total_player_hands = 0;

Shoe shoe = {};
DealerHand dealer_hand = {};
PlayerHand player_hands[MAX_PLAYER_HANDS];

void deal_new_hand()
{
  if(need_to_shuffle(&shoe, num_decks))
  {
    shuffle(&shoe, num_decks);
  }
  
  PlayerHand player_hand = {};
  player_hands[0] = player_hand;
  current_player_hand = 0;

  player_deal_card(&player_hand, &shoe);
  dealer_deal_card(&dealer_hand, &shoe);
  player_deal_card(&player_hand, &shoe);
  dealer_deal_card(&dealer_hand, &shoe);

  if(dealer_upcard_is_ace(&dealer_hand))
  {
    draw_hands();
    ask_insurance();
    return;
  }

  if(player_is_done(&player_hand))
  {
    dealer_hand.hide_down_card = false;    
    pay_hands();
    draw_hands();
    bet_options();
    return;
  }

  draw_hands();
  player_get_action(&player_hand);
  save_game();
}

void draw_hands()
{

}

void ask_insurance()
{

}

void pay_hands()
{

}

void bet_options()
{

}

void save_game()
{

}

int main()
{
  srand(time(NULL));
  bt_off();
  new_regular(&shoe, num_decks);
  shuffle(&shoe, num_decks);
  deal_new_hand(&shoe, &dealer_hand, player_hands);
  bt_on();

  return 0;
}

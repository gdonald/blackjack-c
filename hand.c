#include "hand.h"

bool is_busted(Hand *hand)
{
  return get_value(hand, Soft) > 21;
}

bool is_blackjack(Hand *hand)
{
  if(hand->num_cards != 2)
  {
    return false;
  }

  if(is_ace(&hand->cards[0]) && is_ten(&hand->cards[1]))
  {
    return true;
  }

  if(is_ace(&hand->cards[1]) && is_ten(&hand->cards[0]))
  {
    return true;
  }

  return false;
}

bool is_done(Hand *hand)
{
  if(is_busted(hand))
  {
    return true;
  }

  return false;
}

unsigned get_value(Hand *hand, CountMethod method)
{

}

void deal_card(Hand *hand, Shoe *shoe)
{

}

void draw(Hand *hand)
{

}

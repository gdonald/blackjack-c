#include "player_hand.h"

bool ph_is_busted(PlayerHand *hand)
{
  return ph_get_value(hand, Soft) > 21;
}

bool ph_is_blackjack(PlayerHand *hand)
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

bool ph_is_done(PlayerHand *hand)
{
  if(ph_is_busted(hand))
  {
    return true;
  }

  return false;
}

unsigned ph_get_value(PlayerHand *hand, CountMethod method)
{
  unsigned v = 0;
  unsigned total = 0;

  for(unsigned x = 0; x < hand->num_cards; ++x)
  {
    unsigned tmp_v = hand->cards[x].value + 1;
    v = tmp_v > 9 ? 10 : tmp_v;

    if(method == Soft && v == 1 && total < 11)
    {
      v = 11;
    }

    total += v;
  }

  if(method == Soft && total > 21)
  {
    return ph_get_value(hand, Hard);
  }

  return total;
}

void ph_deal_card(PlayerHand *hand, Shoe *shoe)
{
  Card c = get_next_card(shoe);
  hand->cards[hand->num_cards++] = c;
}

void ph_draw(PlayerHand *hand)
{
  printf("%d", ph_get_value(hand, Soft));
}

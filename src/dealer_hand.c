#include "dealer_hand.h"

void dealer_deal_card(DealerHand *hand)
{
  Card c = deal_card();
  hand->cards[hand->num_cards++] = c;
}

bool dealer_upcard_is_ace(DealerHand *dealer_hand)
{
  return card_is_ace(&dealer_hand->cards[0]);
}

bool dealer_is_blackjack(DealerHand *hand)
{
  if(hand->num_cards != 2)
  {
    return false;
  }

  if(card_is_ace(&hand->cards[0]) && card_is_ten(&hand->cards[1]))
  {
    return true;
  }

  if(card_is_ace(&hand->cards[1]) && card_is_ten(&hand->cards[0]))
  {
    return true;
  }

  return false;
}

unsigned dealer_get_value(CountMethod method)
{
  unsigned v = 0;
  unsigned total = 0;

  for(unsigned x = 0; x < dealer_hand.num_cards; ++x)
  {
    if(x == 1 && dealer_hand.hide_down_card)
    {
      continue;
    }

    unsigned tmp_v = dealer_hand.cards[x].value + 1;
    v = tmp_v > 9 ? 10 : tmp_v;

    if(method == Soft && v == 1 && total < 11)
    {
      v = 11;
    }

    total += v;
  }

  if(method == Soft && total > 21)
  {
    return dealer_get_value(Hard);
  }

  return total;
}

bool dealer_is_busted()
{
  return dealer_get_value(Soft) > 21;
}

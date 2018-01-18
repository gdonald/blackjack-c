#include "dealer_hand.h"

void dealer_deal_card(DealerHand *hand, Shoe *shoe)
{
  Card c = deal_card(shoe);
  hand->cards[hand->num_cards++] = c;
}

bool dealer_upcard_is_ace(DealerHand *dealer_hand)
{
  return card_is_ace(&dealer_hand->cards[0]);
}

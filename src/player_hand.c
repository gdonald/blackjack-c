#include "player_hand.h"

bool player_is_busted(PlayerHand *hand)
{
  return player_get_value(hand, Soft) > 21;
}

bool player_is_blackjack(PlayerHand *hand)
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

bool player_is_done(PlayerHand *hand)
{
  if(player_is_busted(hand))
  {
    return true;
  }

  return false;
}

unsigned player_get_value(PlayerHand *hand, CountMethod method)
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
    return player_get_value(hand, Hard);
  }

  return total;
}

void player_deal_card(PlayerHand *hand, Shoe *shoe)
{
  Card c = deal_card(shoe);
  hand->cards[hand->num_cards++] = c;
}

void player_draw(PlayerHand *hand)
{
  printf("%d", player_get_value(hand, Soft));
}

void player_get_action(PlayerHand *hand)
{
  printf(" ");

  if(player_can_hit(hand))
  {
    printf("(H) Hit  ");
  }

  // TODO:
}

bool player_can_hit(PlayerHand *hand)
{
  if(hand->played || hand->stood || 21 == player_get_value(hand, Hard) || player_is_blackjack(hand) || player_is_busted(hand))
  {
    return false;
  }

  return true;
}

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

void player_deal_card(PlayerHand *hand)
{
  Card c = deal_card();
  hand->cards[hand->num_cards++] = c;
}

void player_get_action(PlayerHand *hand)
{
  printf(" ");

  if(player_can_hit(hand))
  {
    printf("(H) Hit  ");
  }

  if(player_can_stand(hand))
  {
    printf("(S) Stand  ");
  }

  if(player_can_split(hand))
  {
    printf("(P) Split  ");
  }

  if(player_can_dbl(hand))
  {
    printf("(D) Double  ");
  }

  printf("\n");

  bool br = false;
  char c = { 0 };

  while(true)
  {
    c = getchar();
    
    switch(c)
    {
    case 'h':
      br = true;
      player_hit(hand);
      break;

    case 's':
      br = true;
      player_stand(hand);
      break;

    case 'p':
      br = true;
      player_split(hand);
      break;

    case 'd':
      br = true;
      player_dbl(hand);
      break;

    default:
      br = true;
      clear();
      draw_hands();
      player_get_action(hand);
    }

    if(br)
    {
      break;
    }
  }
}

bool player_can_hit(PlayerHand *hand)
{
  if(hand->played || hand->stood || 21 == player_get_value(hand, Hard) || player_is_blackjack(hand) || player_is_busted(hand))
  {
    return false;
  }

  return true;
}

bool player_can_stand(PlayerHand *hand)
{
  if(hand->stood || player_is_busted(hand) || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

bool player_can_split(PlayerHand *hand)
{
  if(hand->stood || total_player_hands >= MAX_PLAYER_HANDS)
  {
    return false;
  }

  if(money < all_bets() + hand->bet)
  {
    return false;
  }

  if(hand->num_cards == 2 && hand->cards[0].value == hand->cards[1].value)
  {
    return true;
  }

  return false;
}

bool player_can_dbl(PlayerHand *hand)
{
  if(money < all_bets() + hand->bet)
  {
    return false;
  }

  if(hand->stood || hand->num_cards != 2 || player_is_busted(hand) || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

void player_hit(PlayerHand *hand)
{
  player_deal_card(hand);

  if(player_is_done(hand))
  {
    process();
    return;
  }

  draw_hands();
  player_get_action(&player_hands[current_player_hand]);
}

void player_stand(PlayerHand *hand)
{
  hand->stood = true;
  hand->played = true;

  if(more_hands_to_play())
  {
    play_more_hands();
    return;
  }

  play_dealer_hand();
  draw_hands();
  bet_options();
}

void player_split(PlayerHand *hand)
{
  if(!player_can_split(hand))
  {
    draw_hands();
    player_get_action(hand);
    return;
  }

  PlayerHand new_hand = {};
  player_hands[current_player_hand + 1] = new_hand;
  ++total_player_hands;

  // expand hands
  unsigned x = total_player_hands - 1;
  while(x > current_player_hand)
  {
    player_hands[x] = player_hands[x - 1];
    --x;
  }

  // split
  PlayerHand *this_hand = &player_hands[current_player_hand];
  PlayerHand *split_hand = &player_hands[current_player_hand + 1];

  Card c = this_hand->cards[1];
  split_hand->cards[0] = c;
  split_hand->num_cards = 1;
  this_hand->num_cards = 1;

  player_deal_card(this_hand);

  if(player_is_done(this_hand))
  {
    process();
    return;
  }

  draw_hands();
  player_get_action(&player_hands[current_player_hand]);
}

void player_dbl(PlayerHand *hand)
{
  player_deal_card(hand);
  hand->played = true;
  hand->bet *= 2;

  if(player_is_done(hand))
  {
    process();
  }
}

void player_draw_hand(unsigned index)
{
  //printf("total_player_hands: %d\n", total_player_hands);
  
  PlayerHand *hand = &player_hands[index];

  printf(" ");

  printf("hand->num_cards: %d", hand->num_cards);
  
  for(unsigned i = 0; i < hand->num_cards; ++i)
  {
    Card *c = &hand->cards[i];
    printf("%s ", card_faces[c->value][c->suite_value]);
  }

  printf(" ⇒  %d  ", player_get_value(hand, Soft));

  if(hand->status == Lost)
  {
    printf("-");
  }
  else if(hand->status == Won)
  {
    printf("+");
  }

  printf("$%.2f", (float)(hand->bet / 100.0));

  if(!hand->played && index == current_player_hand)
  {
    printf(" ⇐");
  }

  printf("  ");

  if(hand->status == Lost)
  {
    if(player_is_busted(hand))
    {
      printf("Busted!");
    }
    else
    {
      printf("Lose!");
    }
  }
  else if(hand->status == Won)
  {
    if(player_is_blackjack(hand))
    {
      printf("Blackjack!");
    }
    else
    {
      printf("Won!");
    }
  }
  else if(hand->status == Push)
  {
    printf("Push");
  }
  
  printf("\n\n");
}
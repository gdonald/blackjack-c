#include "shoe.h"

const unsigned shuffle_specs[8][2] = { { 95, 8 },
				       { 92, 7 },
				       { 89, 6 },
				       { 86, 5 },
				       { 84, 4 },
				       { 82, 3 },
				       { 81, 2 },
				       { 80, 1 } };

bool need_to_shuffle()
{
  unsigned total_cards = num_decks * CARDS_PER_DECK;
  unsigned used = (shoe.current_card / (double) total_cards) * 100.0;

  for(unsigned x = 0; x < 8; ++x)
  {
    if(num_decks == shuffle_specs[x][1] && used > shuffle_specs[x][0])
    {
      return true;
    }
  }
  
  return false;
}

void shuffle()
{
  for(unsigned i = (num_decks * CARDS_PER_DECK) - 1; i > 0; i--)
  {
    unsigned j = rand() % (i + 1);
    swap(&shoe.cards[i], &shoe.cards[j]);
  }

  shoe.current_card = 0;
}

void swap(Card *a, Card *b)
{
  Card tmp = *a;
  *a = *b;
  *b = tmp;
}

void new_regular()
{
  unsigned x = 0;

  for(unsigned deck = 0; deck < num_decks; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      for(unsigned value = 0; value < 13; ++value)
      {
	Card c = { value, suite, card_suites[suite] };
	shoe.cards[x++] = c;
      }
    }
  }
}

Card deal_card()
{
  return shoe.cards[shoe.current_card++];
}

#include "card.h"

bool is_ace(Card *card)
{
  return card->value == 0;
}

bool is_ten(Card *card)
{
  return card->value > 9;
}

char *to_s(Card *card)
{
  return faces[card->value][card->suite_value];
}

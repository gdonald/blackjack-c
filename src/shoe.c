#include "shoe.h"

const unsigned shuffle_specs[8][2] = { { 95, 8 },
				       { 92, 7 },
				       { 89, 6 },
				       { 86, 5 },
				       { 84, 4 },
				       { 82, 3 },
				       { 81, 2 },
				       { 80, 1 } };

bool need_to_shuffle(Shoe *shoe)
{
  // TODO:
  if(shoe->current_card > 0)
  {
    return true;
  }

  return false;
}

void shuffle(Shoe *shoe)
{
  shoe->current_card = 0;
}

void new_regular(Shoe *shoe)
{
  shoe->current_card = 0;
}

Card get_next_card(Shoe *shoe)
{ 
  return shoe->cards[shoe->current_card++];
}

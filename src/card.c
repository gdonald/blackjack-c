#include "card.h"

char *card_faces[14][4] = { { "🂡", "🂱", "🃁", "🃑" },
		       { "🂢", "🂲", "🃂", "🃒" },
		       { "🂣", "🂳", "🃃", "🃓" },
		       { "🂤", "🂴", "🃄", "🃔" },
                       { "🂥", "🂵", "🃅", "🃕" },
                       { "🂦", "🂶", "🃆", "🃖" },
                       { "🂧", "🂷", "🃇", "🃗" },
                       { "🂨", "🂸", "🃈", "🃘" },
                       { "🂩", "🂹", "🃉", "🃙" },
                       { "🂪", "🂺", "🃊", "🃚" },
                       { "🂫", "🂻", "🃋", "🃛" },
                       { "🂭", "🂽", "🃍", "🃝" },
                       { "🂮", "🂾", "🃎", "🃞" },
                       { "🂠", "",  "",  ""  }, };

char *card_suites[4] = { "Spades", "Hearts", "Clubs", "Diamonds" };

bool card_is_ace(Card *card)
{
  return card->value == 0;
}

bool card_is_ten(Card *card)
{
  return card->value > 9;
}

char *card_to_string(Card *card)
{
  return card_faces[card->value][card->suite_value];
}

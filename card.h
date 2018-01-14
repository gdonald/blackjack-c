#ifndef CARD_H
#define CARD_H

#include <stdbool.h>

char *faces[14][4] = { { "🂡", "🂱", "🃁", "🃑" },
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

char *suites[4] = { "Spades", "Hearts", "Clubs", "Diamonds" };

struct card
{
  int value;
  int suite_value;
  char *suite;
};

typedef struct card Card;

bool is_ace(Card *card);
bool is_ten(Card *card);
char *to_s(Card *card);

#endif

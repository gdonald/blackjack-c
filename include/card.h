#ifndef CARD_H
#define CARD_H

#include <stdbool.h>

char *card_faces[14][4];
char *card_suites[4];

typedef struct
{
  unsigned value;
  unsigned suite_value;
  char *suite;
} Card;

bool card_is_ace(Card *card);
bool card_is_ten(Card *card);
char *card_to_string(Card *card);

#endif

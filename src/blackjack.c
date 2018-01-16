#include "game.h"

int main()
{
  srand(time(NULL));
  int status = 0;
  bt_off();
  status = run(&shoe, &dealer_hand, player_hands);
  bt_on();

  return status;
}

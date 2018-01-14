#include "buffer_toggle.h"
#include "game.h"

int main()
{
  srand(time(NULL));
  int status = 0;

  bt_off();

  /*
  shoe = {};
  dealer_hand = {};
  player_hands[MAX_PLAYER_HANDS] = Hand[];
  */
  
  status = run(&shoe, &dealer_hand, player_hands);
  bt_on();

  return status;
}

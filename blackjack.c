#include "buffer_toggle.h"
#include "game.h"

int main()
{
  srand(time(NULL));
  int status = 0;

  bt_off();
  status = run();
  bt_on();

  return status;
}

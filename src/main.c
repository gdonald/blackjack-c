
#include "blackjack.h"

int main(int argc, char *argv[]) {
  struct timeval time;
  gettimeofday(&time, NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

  struct Game game = {
    .num_decks = 8,
    .face_type = 1,
    .deck_type = 1,
    .money = 10000,
    .current_bet = 500,
    .shuffle_specs = shuffle_specs,
    .faces = faces,
    .faces2 = faces2
  };

  load_game(&game);

  struct termios term;
  buffer_off(&term);

  while (!game.quitting) {
    deal_new_hand(&game);
  }

  buffer_on(&term);

  return 0;
}

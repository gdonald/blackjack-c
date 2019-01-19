
#include "blackjack.h"

int main(int argc, char *argv[])
{
  struct arguments arguments = { .players = 1 };
  srand((unsigned)time(NULL));

  struct Game game = { .num_decks = 8,
		       .money = 10000,
		       .current_bet = 500,
		       .shuffle_specs = shuffle_specs,
		       .card_faces = card_faces,
		       .num_players = arguments.players };

  load_game(&game);
  new_regular(&game);

  struct termios term;
  buffer_off(&term);
  deal_new_hand(&game);
  buffer_on(&term);

  return 0;
}

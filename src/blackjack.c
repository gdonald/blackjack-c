
#include "blackjack.h"

Card deal_card(Shoe *shoe)
{
  return shoe->cards[shoe->current_card++];
}

bool card_is_ace(Card *card)
{
  return card->value == 0;
}

bool card_is_ten(Card *card)
{
  return card->value > 9;
}

unsigned player_get_value(PlayerHand *hand, CountMethod method)
{
  unsigned v = 0;
  unsigned total = 0;

  for(unsigned x = 0; x < hand->num_cards; ++x)
  {
    unsigned tmp_v = hand->cards[x].value + 1;
    v = tmp_v > 9 ? 10 : tmp_v;

    if(method == Soft && v == 1 && total < 11)
    {
      v = 11;
    }

    total += v;
  }

  if(method == Soft && total > 21)
  {
    return player_get_value(hand, Hard);
  }

  return total;
}

bool player_is_busted(PlayerHand *hand)
{
  return player_get_value(hand, Soft) > 21;
}

bool player_is_blackjack(PlayerHand *hand)
{
  if(hand->num_cards != 2)
  {
    return false;
  }

  if(card_is_ace(&hand->cards[0]) && card_is_ten(&hand->cards[1]))
  {
    return true;
  }

  if(card_is_ace(&hand->cards[1]) && card_is_ten(&hand->cards[0]))
  {
    return true;
  }

  return false;
}

bool player_can_hit(PlayerHand *hand)
{
  if(hand->played
     || hand->stood
     || 21 == player_get_value(hand, Hard)
     || player_is_blackjack(hand)
     || player_is_busted(hand))
  {
    return false;
  }

  return true;
}

bool player_can_stand(PlayerHand *hand)
{
  if(hand->stood
     || player_is_busted(hand)
     || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

unsigned all_bets(Game *game)
{
  unsigned bets = 0;

  for(unsigned x = 0; x < game->total_player_hands; x++)
  {
    bets += game->player_hands[x].bet;
  }

  return bets;
}

bool player_can_split(Game *game)
{ 
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  if(hand->stood || game->total_player_hands >= MAX_PLAYER_HANDS)
  {
    return false;
  }

  if(game->money < all_bets(game) + hand->bet)
  {
    return false;
  }

  if(hand->num_cards == 2 && hand->cards[0].value == hand->cards[1].value)
  {
    return true;
  }

  return false;
}

bool player_can_dbl(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  if(game->money < all_bets(game) + hand->bet)
  {
    return false;
  }

  if(hand->stood
     || hand->num_cards != 2
     || player_is_busted(hand)
     || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

void player_deal_card(Shoe *shoe, PlayerHand *hand)
{
  hand->cards[hand->num_cards++] = deal_card(shoe);
}

bool player_is_done(Game *game, PlayerHand *hand)
{
  if(hand->played
     || hand->stood
     || player_is_blackjack(hand)
     || player_is_busted(hand)
     || 21 == player_get_value(hand, Soft)
     || 21 == player_get_value(hand, Hard))
  {
    hand->played = true;

    if(!hand->payed && player_is_busted(hand))
    {
      hand->payed = true;
      hand->status = Lost;
      game->money -= hand->bet;
    }

    return true;
  }

  return false;
}

bool more_hands_to_play(Game *game)
{
  return game->current_player_hand < game->total_player_hands - 1;
}

bool dealer_is_blackjack(DealerHand *hand)
{
  if(hand->num_cards != 2)
  {
    return false;
  }

  if(card_is_ace(&hand->cards[0]) && card_is_ten(&hand->cards[1]))
  {
    return true;
  }

  if(card_is_ace(&hand->cards[1]) && card_is_ten(&hand->cards[0]))
  {
    return true;
  }

  return false;
}

bool need_to_play_dealer_hand(Game *game)
{
  for(unsigned x = 0; x < game->total_player_hands; ++x)
  {
    PlayerHand *hand = &game->player_hands[x];

    if(!(player_is_busted(hand) || player_is_blackjack(hand)))
    {
      return true;
    }
  }

  return false;
}

unsigned dealer_get_value(DealerHand *hand, CountMethod method)
{
  unsigned v = 0;
  unsigned total = 0;

  for(unsigned x = 0; x < hand->num_cards; ++x)
  {
    if(x == 1 && hand->hide_down_card)
    {
      continue;
    }

    unsigned tmp_v = hand->cards[x].value + 1;
    v = tmp_v > 9 ? 10 : tmp_v;

    if(method == Soft && v == 1 && total < 11)
    {
      v = 11;
    }

    total += v;
  }

  if(method == Soft && total > 21)
  {
    return dealer_get_value(hand, Hard);
  }

  return total;
}

bool dealer_is_busted(DealerHand *hand)
{
  return dealer_get_value(hand, Soft) > 21;
}

void normalize_bet(Game *game)
{
  if(game->current_bet < MIN_BET)
  {
    game->current_bet = MIN_BET;
  }
  else if(game->current_bet > MAX_BET)
  {
    game->current_bet = MAX_BET;
  }
  
  if(game->current_bet > game->money)
  {
    game->current_bet = game->money;
  }
}

void save_game(Game *game)
{
  FILE *fp = fopen(SAVE_FILE, "w");

  if(fp != NULL)
  {
    fprintf(fp, "%d\n%d\n%d\n", game->num_decks, game->money, game->current_bet);
    fclose(fp);
  }
}

void load_game(Game *game)
{
  FILE *fp = fopen(SAVE_FILE, "r");

  if(fp != NULL)
  {
    char buffer[32];

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%u", &game->num_decks);

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%d", &game->money);

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%d", &game->current_bet);

    fclose(fp);
  }
}

void pay_hands(Game *game)
{
  DealerHand *dealer_hand = &game->dealer_hand;
  unsigned dhv = dealer_get_value(dealer_hand, Soft);
  bool dhb = dealer_is_busted(dealer_hand);

  for(unsigned x = 0; x < game->total_player_hands; ++x)
  {
    PlayerHand *hand = &game->player_hands[x];

    if(hand->payed)
    {
      continue;
    }

    hand->payed = true;

    unsigned phv = player_get_value(hand, Soft);

    if(dhb || phv > dhv)
    {
      if(player_is_blackjack(hand))
      {
	hand->bet *= 1.5;
      }

      game->money += hand->bet;
      hand->status = Won;
    }
    else if(phv < dhv)
    {
      game->money -= hand->bet;
      hand->status = Lost;
    }
    else
    {
      hand->status = Push;
    }
  }

  normalize_bet(game);
  save_game(game);
}

void dealer_deal_card(Shoe *shoe, DealerHand *hand)
{
  hand->cards[hand->num_cards++] = deal_card(shoe);
}

void play_dealer_hand(Game *game)
{
  DealerHand *hand = &game->dealer_hand;

  if(dealer_is_blackjack(hand))
  {
    hand->hide_down_card = false;
  }

  if(!need_to_play_dealer_hand(game))
  {
    hand->played = true;
    pay_hands(game);
    return;
  }

  hand->hide_down_card = false;

  unsigned soft_count = dealer_get_value(hand, Soft);
  unsigned hard_count = dealer_get_value(hand, Hard);

  while(soft_count < 18 && hard_count < 17)
  {
    dealer_deal_card(&game->shoe, hand);
    soft_count = dealer_get_value(hand, Soft);
    hard_count = dealer_get_value(hand, Hard);
  }

  hand->played = true;
  pay_hands(game);
}

void clear()
{
  system("export TERM=linux; clear");
}

void draw_dealer_hand(Game *game)
{
  DealerHand *hand = &game->dealer_hand;

  printf(" ");

  for(unsigned i = 0; i < hand->num_cards; ++i)
  {
    if(i == 1 && hand->hide_down_card)
    {
      printf("%s ", (*game->card_faces)[13][0]);
    }
    else
    {
      Card *card = &hand->cards[i];
      printf("%s ", (*game->card_faces)[card->value][card->suite]);
    }
  }

  printf(" ⇒  %d", dealer_get_value(hand, Soft));
}

void player_draw_hand(Game *game, unsigned index)
{
  PlayerHand *hand = &game->player_hands[index];

  printf(" ");

  for(unsigned i = 0; i < hand->num_cards; ++i)
  {
    Card *c = &hand->cards[i];
    printf("%s ", (*game->card_faces)[c->value][c->suite]);
  }

  printf(" ⇒  %d  ", player_get_value(hand, Soft));

  if(hand->status == Lost)
  {
    printf("-");
  }
  else if(hand->status == Won)
  {
    printf("+");
  }

  printf("$%.2f", (float)(hand->bet / 100.0));

  if(!hand->played && index == game->current_player_hand)
  {
    printf(" ⇐");
  }

  printf("  ");

  if(hand->status == Lost)
  {
    printf(player_is_busted(hand) ? "Busted!" : "Lose!");
  }
  else if(hand->status == Won)
  {
    printf(player_is_blackjack(hand) ? "Blackjack!" : "Won!");
  }
  else if(hand->status == Push)
  {
    printf("Push");
  }
  
  printf("\n\n");
}

void draw_hands(Game *game)
{
  clear();
  printf("\n Dealer: \n");
  draw_dealer_hand(game);
  printf("\n\n Player $%.2f:\n", (float)(game->money / 100.0));

  for(unsigned x = 0; x < game->total_player_hands; x++)
  {
    player_draw_hand(game, x);
  }
}

bool need_to_shuffle(Game *game)
{
  unsigned used = (game->shoe.current_card / (double) game->shoe.num_cards) * 100.0;

  for(unsigned x = 0; x < MAX_DECKS; ++x)
  {
    if(game->num_decks == (*game->shuffle_specs)[x][1] && used > (*game->shuffle_specs)[x][0])
    {
      return true;
    }
  }
  
  return false;
}

void swap(Card *a, Card *b)
{
  Card tmp = *a;
  *a = *b;
  *b = tmp;
}

void shuffle(Shoe *shoe)
{
  for(unsigned x = 0; x < 7; x++)
  {
    for(unsigned i = shoe->num_cards - 1; i > 0; i--)
    {
      unsigned j = rand() % (i + 1);
      swap(&shoe->cards[i], &shoe->cards[j]);
    }
  }

  shoe->current_card = 0;
}

bool dealer_upcard_is_ace(DealerHand *hand)
{
  return card_is_ace(&hand->cards[0]);
}

void insure_hand(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  hand->bet /= 2;
  hand->played = true;
  hand->payed = true;
  hand->status = Lost;
  
  game->money -= hand->bet;
  
  draw_hands(game);
  bet_options(game);
}

void no_insurance(Game *game)
{
  DealerHand *dealer_hand = &game->dealer_hand;

  if(dealer_is_blackjack(dealer_hand))
  {
    dealer_hand->hide_down_card = false;
    dealer_hand->played = true;

    pay_hands(game);
    draw_hands(game);
    bet_options(game);
    return;
  }
  
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  if(player_is_done(game, hand))
  {
    play_dealer_hand(game);
    draw_hands(game);
    bet_options(game);
    return;
  }
  
  draw_hands(game);
  player_get_action(game);
}

void ask_insurance(Game *game)
{
  printf(" Insurance?  (Y) Yes  (N) No\n");
  bool br = false;
  char c = { 0 };

  while(true)
  {
    c = getchar();

    switch(c)
    {
    case 'y':
      br = true;
      insure_hand(game);
      break;
    case 'n':
      br = true;
      no_insurance(game);
      break;
    default:
      br = true;
      clear();
      draw_hands(game);
      ask_insurance(game);
      break;
    }

    if(br) break;
  }
}

void deal_new_hand(Game *game)
{
  Shoe *shoe = &game->shoe;

  if(need_to_shuffle(game))
  {
    shuffle(shoe);
  }
  
  PlayerHand player_hand = { .bet=game->current_bet };
  DealerHand *dealer_hand = &game->dealer_hand;

  dealer_hand->hide_down_card = true;
  dealer_hand->num_cards = 0;

  player_deal_card(shoe, &player_hand);
  dealer_deal_card(shoe, dealer_hand);
  player_deal_card(shoe, &player_hand);
  dealer_deal_card(shoe, dealer_hand);

  game->player_hands[0] = player_hand;
  game->current_player_hand = 0;
  game->total_player_hands = 1;
  
  if(dealer_upcard_is_ace(dealer_hand))
  {
    draw_hands(game);
    ask_insurance(game);
    return;
  }

  if(player_is_done(game, &player_hand))
  {
    dealer_hand->hide_down_card = false;    
    pay_hands(game);
    draw_hands(game);
    bet_options(game);
    return;
  }

  draw_hands(game);
  player_get_action(game);
  save_game(game);
}

void get_new_bet(Game *game)
{
  clear();
  draw_hands(game);

  printf("  Current Bet: $%d  Enter New Bet: $", (game->current_bet / 100));

  unsigned tmp;
  scanf("%d", &tmp);

  game->current_bet = tmp * 100;
  normalize_bet(game);
  deal_new_hand(game);
}

void bet_options(Game *game)
{
  printf(" (D) Deal Hand  (B) Change Bet  (Q) Quit\n");

  bool br = false;
  char c = { 0 };

  while(true)
  {
    c = getchar();

    switch(c)
    {
    case 'd':
      br = true;
      deal_new_hand(game);
      break;
    case 'b':
      br = true;
      get_new_bet(game);
      break;
    case 'q':
      clear();
      br = true;
      break;
    default:
      br = true;
      clear();
      draw_hands(game);
      bet_options(game);
    }

    if(br) break;
  }
}

void process(Game *game)
{
  if(more_hands_to_play(game))
  {
    play_more_hands(game);
    return;
  }

  play_dealer_hand(game);
  draw_hands(game);
  bet_options(game);
}

void play_more_hands(Game *game)
{
  PlayerHand *hand = &game->player_hands[++(game->current_player_hand)];
  player_deal_card(&game->shoe, hand);

  if(player_is_done(game, hand))
  {
    process(game);
    return;
  }

  draw_hands(game);
  player_get_action(game);
}

void player_hit(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];
  player_deal_card(&game->shoe, hand);

  if(player_is_done(game, hand))
  {
    process(game);
    return;
  }
  
  draw_hands(game);
  player_get_action(game);
}

void player_stand(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  hand->stood = true;
  hand->played = true;

  if(more_hands_to_play(game))
  {
    play_more_hands(game);
    return;
  }

  play_dealer_hand(game);
  draw_hands(game);
  bet_options(game);
}

void player_split(Game *game)
{
  if(!player_can_split(game))
  {
    draw_hands(game);
    player_get_action(game);
    return;
  }

  // add a new hand at the end
  PlayerHand new_hand = { .bet=game->current_bet };
  game->player_hands[game->total_player_hands++] = new_hand;

  // expand hands where split occured
  unsigned x = game->total_player_hands - 1;
  while(x > game->current_player_hand)
  {
    game->player_hands[x] = game->player_hands[x - 1];
    --x;
  }

  // split current hand
  PlayerHand *this_hand = &game->player_hands[game->current_player_hand];
  PlayerHand *split_hand = &game->player_hands[game->current_player_hand + 1];

  Card c = this_hand->cards[1];
  split_hand->cards[0] = c;
  split_hand->num_cards = 1;
  this_hand->num_cards = 1;

  player_deal_card(&game->shoe, this_hand);

  if(player_is_done(game, this_hand))
  {
    process(game);
    return;
  }

  draw_hands(game);
  player_get_action(game);
}

void player_dbl(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  player_deal_card(&game->shoe, hand);
  hand->played = true;
  hand->bet *= 2;

  if(player_is_done(game, hand))
  {
    process(game);
  }
}

char *card_to_string(Game *game, Card *card)
{
  return (*game->card_faces)[card->value][card->suite];
}

void player_get_action(Game *game)
{
  PlayerHand *hand = &game->player_hands[game->current_player_hand];

  printf(" ");

  if(player_can_hit(hand))   printf("(H) Hit  ");
  if(player_can_stand(hand)) printf("(S) Stand  ");
  if(player_can_split(game)) printf("(P) Split  ");
  if(player_can_dbl(game))   printf("(D) Double  ");

  printf("\n");

  bool br = false;
  char c = { 0 };

  while(true)
  {
    c = getchar();

    switch(c)
    {
    case 'h':
      br = true;
      player_hit(game);
      break;

    case 's':
      br = true;
      player_stand(game);
      break;

    case 'p':
      br = true;
      player_split(game);
      break;

    case 'd':
      br = true;
      player_dbl(game);
      break;

    default:
      br = true;
      clear();
      draw_hands(game);
      player_get_action(game);
    }

    if(br) break;
  }
}

void buffer_off(struct termios *term)
{
  tcgetattr(STDIN_FILENO, term);
  term->c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, term);
}

void buffer_on(struct termios *term)
{
  tcgetattr(STDIN_FILENO, term);
  term->c_lflag |= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, term);
}

void new_regular(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      for(unsigned value = 0; value < 13; ++value)
      {
	Card c = { value, suite };
	game->shoe.cards[game->shoe.num_cards++] = c;
      }
    }
  }
}

/*
void new_aces(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks * 5; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      Card c = { 0, suite };
      game->shoe.cards[game->shoe.num_cards++] = c;
    }
  }
}
*/

/*
void new_aces_jacks(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks * 4; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      Card c1 = { 0, suite };
      game->shoe.cards[game->shoe.num_cards++] = c1;
      Card c2 = { 10, suite };
      game->shoe.cards[game->shoe.num_cards++] = c2;
    }
  }
}
*/

/*
void new_jacks(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks * 5; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      Card c = { 10, suite };
      game->shoe.cards[game->shoe.num_cards++] = c;
    }
  }
}
*/

/*
void new_sevens(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks * 5; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      Card c = { 6, suite };
      game->shoe.cards[game->shoe.num_cards++] = c;
    }
  }
}
*/

/*
void new_eights(Game *game)
{
  game->shoe.num_cards = 0;

  for(unsigned deck = 0; deck < game->num_decks * 5; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      Card c = { 7, suite };
      game->shoe.cards[game->shoe.num_cards++] = c;
    }
  }
}
*/

int main()
{
  srand(time(NULL));

  unsigned shuffle_specs[8][2] = { { 95, 8 },
				   { 92, 7 },
				   { 89, 6 },
				   { 86, 5 },
				   { 84, 4 },
				   { 82, 3 },
				   { 81, 2 },
				   { 80, 1 } };

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
			      { "🂠", "",  "",  ""  } };

  Game game = { .num_decks = 8,
		.money = 10000,
		.current_bet = 500,
		.shuffle_specs = &shuffle_specs,
		.card_faces = &card_faces };

  load_game(&game);

  // shoe type
  new_regular(&game);
  // new_aces(&game);       // test soft/hard count
  // new_aces_jacks(&game); // test blackjack
  // new_jacks(&game);      // test push
  // new_sevens(&game);     // test split
  // new_eights(&game);     // test dealer bust

  shuffle(&game.shoe);

  struct termios term;
  buffer_off(&term);
  deal_new_hand(&game);
  buffer_on(&term);
  
  return 0;
}

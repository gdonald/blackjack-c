
#define CARDS_PER_DECK 52
#define MAX_CARDS_PER_HAND 11
#define MAX_DECKS 8
#define MAX_PLAYER_HANDS 7
#define MIN_BET 500
#define MAX_BET 10000000
#define SAVE_FILE "bj.txt"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const unsigned shuffle_specs[8][2];

char *card_faces[14][4];
char *card_suites[4];

typedef enum { Soft, Hard } CountMethod;
typedef enum { Unknown, Won, Lost, Push } HandStatus;

typedef struct
{
  unsigned value;
  unsigned suite_value;
  char *suite;
} Card;

typedef struct
{
  unsigned current_card;
  Card cards[CARDS_PER_DECK * MAX_DECKS];
} Shoe;

typedef struct
{
  bool played;
  bool hide_down_card;
  unsigned num_cards;
  Card cards[MAX_CARDS_PER_HAND];
} DealerHand;

typedef struct
{
  bool stood;
  bool played;
  bool payed;
  unsigned bet;
  HandStatus status;
  unsigned num_cards;
  Card cards[MAX_CARDS_PER_HAND];
} PlayerHand;

unsigned num_decks = 1;
unsigned money = 10000;
unsigned current_bet = 500;
unsigned current_player_hand = 0;
unsigned total_player_hands = 0;

Shoe shoe = {};
DealerHand dealer_hand = {};
PlayerHand player_hands[MAX_PLAYER_HANDS];

const unsigned shuffle_specs[8][2] = { { 95, 8 },
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
			    { "🂠", "",  "",  ""  }, };

void play_more_hands();
void bet_options();
void player_get_action();

Card deal_card()
{
  return shoe.cards[shoe.current_card++];
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

bool player_can_hit()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  if(hand->played || hand->stood || 21 == player_get_value(hand, Hard) || player_is_blackjack(hand) || player_is_busted(hand))
  {
    return false;
  }

  return true;
}

bool player_can_stand()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  if(hand->stood || player_is_busted(hand) || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

unsigned all_bets()
{
  unsigned bets = 0;

  for(unsigned x = 0; x < total_player_hands; x++)
  {
    bets += player_hands[x].bet;
  }

  return bets;
}

bool player_can_split()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  if(hand->stood || total_player_hands >= MAX_PLAYER_HANDS)
  {
    return false;
  }

  if(money < all_bets() + hand->bet)
  {
    return false;
  }

  if(hand->num_cards == 2 && hand->cards[0].value == hand->cards[1].value)
  {
    return true;
  }

  return false;
}

bool player_can_dbl()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  if(money < all_bets() + hand->bet)
  {
    return false;
  }

  if(hand->stood || hand->num_cards != 2 || player_is_busted(hand) || player_is_blackjack(hand))
  {
    return false;
  }

  return true;
}

void player_deal_card(PlayerHand *hand)
{
  hand->cards[hand->num_cards++] = deal_card();
}

bool player_is_done(PlayerHand *hand)
{
  if(hand->played
     || hand->stood
     || player_is_blackjack(hand)
     || player_is_busted(hand)
     || 21 == player_get_value(hand, Soft)
     || 21 == player_get_value(hand, Hard))
  {
    hand->played = true;

    if(!hand->payed)
    {
      if(player_is_busted(hand))
      {
	hand->payed = true;
	hand->status = Lost;
	money -= hand->bet;
      }
    }

    return true;
  }

  return false;
}

bool more_hands_to_play()
{
  return current_player_hand < total_player_hands - 1;
}

bool dealer_is_blackjack()
{
  if(dealer_hand.num_cards != 2)
  {
    return false;
  }

  if(card_is_ace(&dealer_hand.cards[0]) && card_is_ten(&dealer_hand.cards[1]))
  {
    return true;
  }

  if(card_is_ace(&dealer_hand.cards[1]) && card_is_ten(&dealer_hand.cards[0]))
  {
    return true;
  }

  return false;
}

bool need_to_play_dealer_hand()
{
  for(unsigned x = 0; x < total_player_hands; ++x)
  {
    PlayerHand *hand = &player_hands[x];

    if(!(player_is_busted(hand) || player_is_blackjack(hand)))
    {
      return true;
    }
  }

  return false;
}

unsigned dealer_get_value(CountMethod method)
{
  unsigned v = 0;
  unsigned total = 0;

  for(unsigned x = 0; x < dealer_hand.num_cards; ++x)
  {
    if(x == 1 && dealer_hand.hide_down_card)
    {
      continue;
    }

    unsigned tmp_v = dealer_hand.cards[x].value + 1;
    v = tmp_v > 9 ? 10 : tmp_v;

    if(method == Soft && v == 1 && total < 11)
    {
      v = 11;
    }

    total += v;
  }

  if(method == Soft && total > 21)
  {
    return dealer_get_value(Hard);
  }

  return total;
}

bool dealer_is_busted()
{
  return dealer_get_value(Soft) > 21;
}

void normalize_bet()
{
  if(current_bet < MIN_BET)
  {
    current_bet = MIN_BET;
  }
  else if(current_bet > MAX_BET)
  {
    current_bet = MAX_BET;
  }
  
  if(current_bet > money)
  {
    current_bet = money;
  }
}

void save_game()
{

}

void pay_hands()
{
  unsigned dhv = dealer_get_value(Soft);
  bool dhb = dealer_is_busted();

  for(unsigned x = 0; x < total_player_hands; ++x)
  {
    PlayerHand *hand = &player_hands[x];

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

      money += hand->bet;
      hand->status = Won;
    }
    else if(phv < dhv)
    {
      money -= hand->bet;
      hand->status = Lost;
    }
    else
    {
      hand->status = Push;
    }
  }

  normalize_bet();
  save_game();
}

void dealer_deal_card()
{
  dealer_hand.cards[dealer_hand.num_cards++] = deal_card();
}

void play_dealer_hand()
{
  if(dealer_is_blackjack())
  {
    dealer_hand.hide_down_card = false;
  }

  if(!need_to_play_dealer_hand())
  {
    dealer_hand.played = true;
    pay_hands();
    return;
  }

  dealer_hand.hide_down_card = false;

  unsigned soft_count = dealer_get_value(Soft);
  unsigned hard_count = dealer_get_value(Hard);

  while(soft_count < 18 && hard_count < 17)
  {
    dealer_deal_card();
    soft_count = dealer_get_value(Soft);
    hard_count = dealer_get_value(Hard);
  }

  dealer_hand.played = true;
  pay_hands();
}

void clear()
{
  system("export TERM=linux; clear");
}

void draw_dealer_hand()
{
  printf(" ");

  for(unsigned i = 0; i < dealer_hand.num_cards; ++i)
  {
    if(i == 1 && dealer_hand.hide_down_card)
    {
      printf("%s ", card_faces[13][0]);
    }
    else
    {
      Card *card = &dealer_hand.cards[i];
      printf("%s ", card_faces[card->value][card->suite_value]);
    }
  }

  printf(" ⇒  %d", dealer_get_value(Soft));
}

void player_draw_hand(unsigned index)
{
  PlayerHand *hand = &player_hands[index];

  printf(" ");

  for(unsigned i = 0; i < hand->num_cards; ++i)
  {
    Card *c = &hand->cards[i];
    printf("%s ", card_faces[c->value][c->suite_value]);
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

  if(!hand->played && index == current_player_hand)
  {
    printf(" ⇐");
  }

  printf("  ");

  if(hand->status == Lost)
  {
    if(player_is_busted(hand))
    {
      printf("Busted!");
    }
    else
    {
      printf("Lose!");
    }
  }
  else if(hand->status == Won)
  {
    if(player_is_blackjack(hand))
    {
      printf("Blackjack!");
    }
    else
    {
      printf("Won!");
    }
  }
  else if(hand->status == Push)
  {
    printf("Push");
  }
  
  printf("\n\n");
}

void draw_hands()
{
  clear();
  printf("\n Dealer: \n");
  draw_dealer_hand();
  printf("\n\n Player $%.2f:\n", (float)(money / 100.0));

  for(unsigned x = 0; x < total_player_hands; x++)
  {
    player_draw_hand(x);
  }
}

bool need_to_shuffle()
{
  unsigned total_cards = num_decks * CARDS_PER_DECK;
  unsigned used = (shoe.current_card / (double) total_cards) * 100.0;

  for(unsigned x = 0; x < 8; ++x)
  {
    if(num_decks == shuffle_specs[x][1] && used > shuffle_specs[x][0])
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

void shuffle()
{
  for(unsigned i = (num_decks * CARDS_PER_DECK) - 1; i > 0; i--)
  {
    unsigned j = rand() % (i + 1);
    swap(&shoe.cards[i], &shoe.cards[j]);
  }

  shoe.current_card = 0;
}

bool dealer_upcard_is_ace()
{
  return card_is_ace(&dealer_hand.cards[0]);
}

void insure_hand()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  hand->bet /= 2;
  hand->played = true;
  hand->payed = true;
  hand->status = Lost;
  
  money -= hand->bet;
  
  draw_hands();
  bet_options();
}

void no_insurance()
{
  if(dealer_is_blackjack())
  {
    dealer_hand.hide_down_card = false;
    dealer_hand.played = true;

    pay_hands();
    draw_hands();
    bet_options();
    return;
  }
  
  PlayerHand *hand = &player_hands[current_player_hand];

  if(player_is_done(hand))
  {
    play_dealer_hand();
    draw_hands();
    bet_options();
    return;
  }
  
  draw_hands();
  player_get_action(hand);
}

void ask_insurance()
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
      insure_hand();
      break;
    case 'n':
      br = true;
      no_insurance();
      break;
    default:
      br = true;
      clear();
      draw_hands();
      ask_insurance();
      break;
    }

    if(br)
    {
      break;
    }
  }
}

void deal_new_hand()
{
  if(need_to_shuffle(&shoe, num_decks))
  {
    shuffle(&shoe, num_decks);
  }
  
  PlayerHand player_hand = { .bet=current_bet };

  dealer_hand.hide_down_card = true;
  dealer_hand.num_cards = 0;

  player_deal_card(&player_hand);
  dealer_deal_card();
  player_deal_card(&player_hand);
  dealer_deal_card();

  player_hands[0] = player_hand;
  current_player_hand = 0;
  total_player_hands = 1;
  
  if(dealer_upcard_is_ace(&dealer_hand))
  {
    draw_hands();
    ask_insurance();
    return;
  }

  if(player_is_done(&player_hand))
  {
    dealer_hand.hide_down_card = false;    
    pay_hands();
    draw_hands();
    bet_options();
    return;
  }

  draw_hands();
  player_get_action(&player_hand);
  save_game();
}

void get_new_bet()
{
  clear();
  draw_hands();

  printf("  Current Bet: $%d  Enter New Bet: $", (current_bet / 100));

  unsigned tmp;
  scanf("%d", &tmp);

  current_bet = tmp * 100;
  normalize_bet();
  deal_new_hand();
}

void bet_options()
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
      deal_new_hand();
      break;
    case 'b':
      br = true;
      get_new_bet();
      break;
    case 'q':
      clear();
      br = true;
      break;
    default:
      br = true;
      clear();
      draw_hands();
      bet_options();
    }

    if(br)
    {
      break;
    }
  }
}

void process()
{
  if(more_hands_to_play())
  {
    play_more_hands();
    return;
  }

  play_dealer_hand();
  draw_hands();
  bet_options();
}

void play_more_hands()
{
  PlayerHand *hand = &player_hands[current_player_hand];
  player_deal_card(hand);

  if(player_is_done(hand))
  {
    process();
    return;
  }

  draw_hands();
  player_get_action();
}

void player_hit()
{
  PlayerHand *hand = &player_hands[current_player_hand];
  player_deal_card(hand);

  if(player_is_done(hand))
  {
    process();
    return;
  }
  
  draw_hands();
  player_get_action();
}

void player_stand()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  hand->stood = true;
  hand->played = true;

  if(more_hands_to_play())
  {
    play_more_hands();
    return;
  }

  play_dealer_hand();
  draw_hands();
  bet_options();
}

void player_split()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  if(!player_can_split(hand))
  {
    draw_hands();
    player_get_action(hand);
    return;
  }

  PlayerHand new_hand = {};
  player_hands[current_player_hand + 1] = new_hand;
  ++total_player_hands;

  // expand hands
  unsigned x = total_player_hands - 1;
  while(x > current_player_hand)
  {
    player_hands[x] = player_hands[x - 1];
    --x;
  }

  // split
  PlayerHand *this_hand = &player_hands[current_player_hand];
  PlayerHand *split_hand = &player_hands[current_player_hand + 1];

  Card c = this_hand->cards[1];
  split_hand->cards[0] = c;
  split_hand->num_cards = 1;
  this_hand->num_cards = 1;

  player_deal_card(this_hand);

  if(player_is_done(this_hand))
  {
    process();
    return;
  }

  draw_hands();
  player_get_action(&player_hands[current_player_hand]);
}

void player_dbl()
{
  PlayerHand *hand = &player_hands[current_player_hand];

  player_deal_card(hand);
  hand->played = true;
  hand->bet *= 2;

  if(player_is_done(hand))
  {
    process();
  }
}

char *card_to_string(Card *card)
{
  return card_faces[card->value][card->suite_value];
}

void player_get_action()
{
  printf(" ");

  if(player_can_hit())
  {
    printf("(H) Hit  ");
  }

  if(player_can_stand())
  {
    printf("(S) Stand  ");
  }

  if(player_can_split())
  {
    printf("(P) Split  ");
  }

  if(player_can_dbl())
  {
    printf("(D) Double  ");
  }

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
      player_hit();
      break;

    case 's':
      br = true;
      player_stand();
      break;

    case 'p':
      br = true;
      player_split();
      break;

    case 'd':
      br = true;
      player_dbl();
      break;

    default:
      br = true;
      clear();
      draw_hands();
      player_get_action();
    }

    if(br)
    {
      break;
    }
  }
}

void new_regular()
{
  unsigned x = 0;

  for(unsigned deck = 0; deck < num_decks; ++deck)
  {
    for(unsigned suite = 0; suite < 4; ++suite)
    {
      for(unsigned value = 0; value < 13; ++value)
      {
	Card c = { value, suite, card_suites[suite] };
	shoe.cards[x++] = c;
      }
    }
  }
}

int main()
{
  srand(time(NULL));
  new_regular(&shoe, num_decks);
  shuffle(&shoe, num_decks);
  deal_new_hand(&shoe, &dealer_hand, player_hands);
  
  return 0;
}

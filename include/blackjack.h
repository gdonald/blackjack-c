
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define CARDS_PER_DECK 52
#define MAX_CARDS_PER_HAND 11
#define MAX_DECKS 8
#define MAX_PLAYER_HANDS 7
#define MIN_BET 500
#define MAX_BET 10000000
#define SAVE_FILE "bj.txt"

typedef enum { Soft, Hard } CountMethod;
typedef enum { Won=1, Lost, Push } HandStatus;

typedef struct
{
  unsigned value;
  unsigned suite;
} Card;

typedef struct
{
  unsigned current_card;
  unsigned num_cards;
  Card cards[CARDS_PER_DECK * MAX_DECKS];
} Shoe;

typedef struct
{
  unsigned num_cards;
  Card cards[MAX_CARDS_PER_HAND];
} Hand;

typedef struct
{
  bool played;
  bool hide_down_card;
  Hand hand;
} DealerHand;

typedef struct
{
  bool stood;
  bool played;
  bool payed;
  unsigned bet;
  HandStatus status;
  Hand hand;
} PlayerHand;

typedef struct
{
  unsigned num_decks;
  unsigned money;
  unsigned current_bet;
  unsigned current_player_hand;
  unsigned total_player_hands;
  Shoe shoe;
  DealerHand dealer_hand;
  PlayerHand player_hands[MAX_PLAYER_HANDS];
  unsigned (*shuffle_specs)[8][2];
  char *(*card_faces)[14][4];
} Game;

char *card_to_string(Game *game, Card *card);

bool is_ace(Card *card);
bool is_ten(Card *card);
bool player_is_busted(PlayerHand *player_hand);
bool is_blackjack(Hand *hand);
bool player_can_hit(PlayerHand *player_hand);
bool player_can_stand(PlayerHand *player_hand);
bool player_can_split(Game *game);
bool player_can_dbl(Game *game);
bool player_is_done(Game *game, PlayerHand *player_hand);
bool more_hands_to_play(Game *game);
bool need_to_play_dealer_hand(Game *game);
bool dealer_is_busted(DealerHand *dealer_hand);
bool dealer_upcard_is_ace(DealerHand *dealer_hand);
bool need_to_shuffle(Game *game);

unsigned player_get_value(PlayerHand *player_hand, CountMethod method);
unsigned dealer_get_value(DealerHand *dealer_hand, CountMethod method);
unsigned all_bets(Game *game);

void normalize_bet(Game *game);
void save_game(Game *game);
void load_game(Game *game);
void pay_hands(Game *game);
void deal_card(Shoe *shoe, Hand *hand);
void play_dealer_hand(Game *game);
void draw_dealer_hand(Game *game);
void player_draw_hand(Game *game, unsigned index);
void draw_hands(Game *game);
void swap(Card *a, Card *b);
void shuffle(Shoe *shoe);
void insure_hand(Game *game);
void no_insurance(Game *game);
void ask_insurance(Game *game);
void deal_new_hand(Game *game);
void get_new_bet(Game *game);
void bet_options(Game *game);
void process(Game *game);
void play_more_hands(Game *game);
void player_hit(Game *game);
void player_stand(Game *game);
void player_split(Game *game);
void player_dbl(Game *game);
void player_get_action(Game *game);
void new_regular(Game *game);
void new_aces(Game *game);
void new_aces_jacks(Game *game);
void new_jacks(Game *game);
void new_sevens(Game *game);
void new_eights(Game *game);
void buffer_off(struct termios *term);
void buffer_on(struct termios *term);
void clear();

#include "game.h"

unsigned num_decks = 1;
unsigned money = 10000;
unsigned current_bet = 500;
unsigned current_player_hand = 0;
unsigned total_player_hands = 0;

Shoe shoe = {};
DealerHand dealer_hand = {};
PlayerHand player_hands[MAX_PLAYER_HANDS];

void deal_new_hand()
{
  if(need_to_shuffle(&shoe, num_decks))
  {
    shuffle(&shoe, num_decks);
  }
  
  PlayerHand player_hand = {};
  player_hands[0] = player_hand;
  current_player_hand = 0;
  total_player_hands = 1;

  player_deal_card(&player_hand);
  dealer_deal_card(&dealer_hand);
  player_deal_card(&player_hand);
  dealer_deal_card(&dealer_hand);

  player_hand.num_cards = 2;
  
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

void draw_hands()
{
  clear();
  printf("\n Dealer: \n");
  draw_dealer_hand();
  printf("\n");

  /*
  cout << fixed << setprecision(2);
  cout << endl << " Player $" << (float)(money / 100.0) << ":" << endl;
  for(unsigned i = 0; i < playerHands.size(); ++i)
    {
      playerHands.at(i).draw(i);
    }
  */

  printf("\n Player $%f:\n", (float)(money / 100.0));

  for(unsigned x = 0; x < total_player_hands; ++x)
  {
    player_draw_hand(x);
  }
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

void get_new_bet()
{
  clear();
  draw_hands();

  printf("  Current Bet: $%d  Enter New Bet: $", (current_bet / 100));
  unsigned tmp;
  tmp = scanf("%u", &tmp);
  unsigned bet = tmp * 100;
  current_bet = bet;
  
  normalize_bet();
  deal_new_hand();
}

void save_game()
{

}

void clear()
{
  system("export TERM=linux; clear");
}

bool more_hands_to_play()
{
  return current_player_hand < total_player_hands - 1;
}

void play_more_hands()
{
  PlayerHand *hand = &player_hands[++current_player_hand];
  deal_card(hand);

  if(player_is_done(hand))
  {
    process();
    return;
  }

  draw_hands();
  player_get_action(hand);
}

void play_dealer_hand()
{
  if(dealer_is_blackjack(&dealer_hand))
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
    deal_card(dealer_hand);
    soft_count = dealer_get_value(Soft);
    hard_count = dealer_get_value(Hard);
  }

  dealer_hand.played = true;
  pay_hands();
}

bool need_to_play_dealer_hand()
{
  for(unsigned x = 0; x < total_player_hands; ++x)
  {
    PlayerHand *hand = &player_hands[x];

    if(player_is_busted(hand) || player_is_blackjack(hand))
    {
      return true;
    }
  }

  return false;
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
  if(dealer_is_blackjack(dealer_hand))
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

void draw_dealer_hand()
{
  printf(" ");

  for(unsigned i = 0; i < dealer_hand.num_cards; ++i)
  {
    if(i == 1 && dealer_hand.hide_down_card)
    {
      printf("%s", card_faces[13][0]);
    }
    else
    {
      Card *card = &dealer_hand.cards[i];
      printf("%s", card_faces[card->value][card->suite_value]);
    }
  }

  printf(" ⇒  %d", dealer_get_value(Soft));
}

unsigned all_bets()
{
  return 0;
}

int main()
{
  /*
  srand(time(NULL));
  bt_off();
  new_regular(&shoe, num_decks);
  shuffle(&shoe, num_decks);
  deal_new_hand(&shoe, &dealer_hand, player_hands);
  bt_on();
  */

  //  int c;   
  static struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  
  newt = oldt;

  newt.c_lflag &= ~(ICANON);          

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  //while((c=getchar())!= 'e')      
    //putchar(c);                 

  srand(time(NULL));
  new_regular(&shoe, num_decks);
  shuffle(&shoe, num_decks);
  deal_new_hand(&shoe, &dealer_hand, player_hands);
  
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
  
  return 0;
}

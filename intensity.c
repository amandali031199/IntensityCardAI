// intensity.c
// Assignment 2, COMP1511 18s1: Intensity
//a program AI which plays the card game Intensity when called by the referee
// This program by AMANDA XIAORUI LI (z5206613) on 14/5/18
//
// Version 1.0.1: Minor changes to wording of comments.
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <assert.h>

// SOME USEFUL #defines - YOU WILL NEED MORE

#define ACTION_PLAYER_NAME    0
#define ACTION_DISCARD        1
#define ACTION_PLAY_CARD      2
#define ACTION_UNIT_TESTS     3

#define N_CARDS              40
#define N_CARDS_INITIAL_HAND 10
#define N_PLAYERS             4
#define N_CARDS_DISCARDED     3

#define CARD_MIN             10
#define CARD_MAX             49


// ADD EXTRA #defines HERE
#define BUFFALO 47
#define MIN CALF 30
#define MAX CALF 39
#define TRUE 1
#define FALSE 0
#define MAX_N_ROUNDS 4
#define DISCARDED 90
#define NOT_CHOSEN 0

//FUNCTION PROTOTYPES
void print_player_name(void);
void choose_discards(void);
void choose_card_to_play(void);
void run_unit_tests(void);
void scan_cards(int n_cards, int hand[]);
int last_digit(int num);
int first_digit(int num);
int n_tens(int n_cards, int hand[], int tens);

void discard_high_buffalo(int cards[], int discard[], int *i, int *n);
void discard_high_calves(int cards[], int discard[], int *i, int *n);
int discard_high(int hand[], int digit, int i, int discard[]);
int discard_highest_norm(int hand[], int digit, int i, int discard[]);

int strategy(int cards_mine[], int n_cards_mine, int prev_played[],
	     int n_prev_played, int current_round[], int n_current_round);
void not_first_round(int cards_mine[], int n_cards_mine, int prev_played[],
		     int n_prev_played, int current_round[], 
		     int n_current_round, int *play_card);
void no_calves_prev(int cards_mine[], int n_cards_mine, int prev_played[],
		    int n_prev_played, int current_round[], int n_current_round,
		    int *play_card);
void first_round(int cards_mine[], int n_cards_mine, int prev_played[],
		 int n_prev_played, int current_round[], int n_current_round,
		 int *play_card);
void consider_buffalo(int cards_mine[], int n_cards_mine, int prev_played[],
		      int n_prev_played, int current_round[], 
		      int n_current_round, int *play_card);
void play_same_digit(int cards_mine[], int n_cards_mine, int prev_played[],
		     int n_prev_played, int current_round[], 
		     int n_current_round, int *play_card);
void last_to_play(int cards_mine[], int n_cards_mine, int prev_played[],
	 	  int n_prev_played, int current_round[], int n_current_round, 
	 	  int *play_card);
void not_last_to_play(int cards_mine[], int n_cards_mine, int prev_played[],
		      int n_prev_played, int current_round[], 
		      int n_current_round, int *play_card);

int calf_prev_played(int prev_played[], int n_prev_played);
int only_calf_cards(int cards_mine[], int n_cards_mine);
int middle_low_calf(int cards_mine[], int n_cards_mine);
int high_norm(int cards_mine[], int n_cards_mine);
int have_same_first_digit(int cards_mine[], int n_cards_mine,
			  int current_round[]);
int round_has_penalty(int current_round[], int n_current_round);
int highest_under_known(int cards_mine[], int n_cards_mine, int current_round[],
			int n_current_round);
int highest(int cards_mine[], int n_cards_mine, int current_round[]);
int below_six(int cards_mine[], int digit, int n_cards_mine);
int any_card(int cards_mine[], int n_cards_mine);
int last(int cards_mine[], int n_cards_mine);
int last_norm(int cards_mine[], int n_cards_mine);
int lowest_with_digit(int cards_mine[], int n_cards_mine, int current_round[]);
int highest_buffalo(int cards_mine[], int n_cards_mine, int current_round[]);
int buffalo(int cards_mine[], int n_cards_mine);
int buffalo_higher(int cards_mine[], int n_cards_mine);
int highest_exclude_buffalo(int cards_mine[], int n_cards_mine);
int card_higher_buffalo(int cards_mine[], int n_cards_mine);
int voids(int n_cards_mine, int cards_mine[]);
int card_appears(int card, int hand[], int n_hand);

 void test_discards(int array[]);



int main(void) {

    int which_action = 0;
    scanf("%d", &which_action);
    if (which_action == ACTION_PLAYER_NAME) {
        print_player_name();
    } else if (which_action == ACTION_DISCARD) {
        choose_discards();
    } else if (which_action == ACTION_PLAY_CARD) {
        choose_card_to_play();
    } else {
        run_unit_tests();
    }

    return 0;
}

void print_player_name(void) {
    printf("ayye\n");

}

void choose_discards() {
    int cards[N_CARDS_INITIAL_HAND] = {0};
    scan_cards(N_CARDS_INITIAL_HAND,cards);
   
    // Strategy 
    int discard[N_CARDS_DISCARDED];
    int i = 0;
    int n = N_CARDS_INITIAL_HAND - 1;
    
    discard_high_buffalo(cards,discard, &i, &n);
    discard_high_calves(cards,discard, &i,&n);

    int digit = 9;
    while (digit>= 5){
	i = discard_high(cards,digit,i,discard);
	digit--;
    }
     
    digit = 4;
    while (digit >= 0){
   	i = discard_highest_norm(cards,digit,i,discard);
	digit--;
    }
    //printing cards to discard
    int j = 0;
    while (j < N_CARDS_DISCARDED){
	printf("%d ", discard[j]);
	j++;
    }
    printf("\n");
}


void choose_card_to_play(void) {
    int play_card = 0;
    int n_cards_mine, n_current_round, position;
    scanf("%d %d %d", &n_cards_mine, &n_current_round, &position);

    // scan cards in own hand 
    int cards_mine[n_cards_mine];
    scan_cards(n_cards_mine, cards_mine);

    //scan cards played in current round
    int current_round[MAX_N_ROUNDS] = {0};
    scan_cards(n_current_round, current_round);
 
    // scan cards played in previous rounds
    int n_prev_played = (N_CARDS_INITIAL_HAND - n_cards_mine) * N_PLAYERS;
    int prev_played[N_CARDS_INITIAL_HAND * N_PLAYERS] = {0};
    scan_cards(n_prev_played, prev_played);

    //scan discarded cards 
    int cards_discarded[N_CARDS_DISCARDED];
    scan_cards(N_CARDS_DISCARDED, cards_discarded);

    //scan cards received in discard round
    int cards_received[N_CARDS_DISCARDED];
    scan_cards(N_CARDS_DISCARDED, cards_received);
    
    //strategy 
    play_card = strategy(cards_mine, n_cards_mine, prev_played, n_prev_played,
			 current_round, n_current_round);

    // print card to play
    printf("%d\n", play_card);

}


//I first tested the smaller functions so that it would be easier to spot 
//a problem. I tested the possible cases, edge cases, especially cases involving
//calves and buffalo. For the smaller function tests, I used mainly arrays of 3
//because three elements were enough to test if the functions worked, and having
//more (10 elements) would make the cases more complicated. Occasionally, I 
//switched to 4 elements to make sure the functions work for all sizes. 
//Next I tested the complete strategy and the logical flow from one function 
//to another. I used minimum one case for each path, to check the logic was 
//correct.
//I tested the smaller functions within strategy by making sure they return the 
//same number as the strategy function tests 

void run_unit_tests(void) {
    //TESTING LAST DIGIT
    assert(last_digit(10) == 0);
    assert(last_digit(29) == 9);
    assert(last_digit(45) == 5);
    assert(last_digit(35) == 5);

   //TESTING FIRST DIGIT
    assert(first_digit(10) == 1);
    assert(first_digit(29) == 2);
    assert(first_digit(45) == 4);
    assert(first_digit(35) == 3);

    //TESTING N_TENS
    int array1[10] = {10,13,24,25,26,27,28,46,47,48};
    assert(n_tens(10,array1,1) == 2);
    assert(n_tens(10,array1,2) == 5);
    assert(n_tens(10,array1,3) == 0);
    assert(n_tens(10,array1,4) == 3);
    
    //TESTING CALF_PREV_PLAYED
    int prev_array1[3] = {31,11,12};
    int prev_array2[3] = {11,31,12};
    int prev_array3[3] = {13,11,30};
    int prev_array4[3] = {24,11,12};
    assert(calf_prev_played(prev_array1,3) == TRUE);
    assert(calf_prev_played(prev_array2,3) == TRUE);
    assert(calf_prev_played(prev_array3,3) == TRUE);
    assert(calf_prev_played(prev_array4,3) == FALSE);
    
    //TESTING ONLY_CALF_CARDS
    int array2[3] = {31,32,30};
    int array3[3] = {31,25,32};
    assert(only_calf_cards(prev_array1,3) == FALSE);
    assert(only_calf_cards(prev_array3,3) == FALSE);
    assert(only_calf_cards(array2,3) == TRUE);
    assert(only_calf_cards(array3,3) == FALSE);
   
    //TESTING MIDDLE_LOW_CALF
    int array4[3] = {34,35,36};
    int array5[3] = {34,33,32};
    int array6[3] = {30,31,32};
    int array7[3] = {37,34,33};
    int array8[3] = {36,37,38};
    int array9[3] = {39,38,37};
    assert(middle_low_calf(array4,3) == 36);
    assert(middle_low_calf(array5,3) == 34);
    assert(middle_low_calf(array6,3) == 32);
    assert(middle_low_calf(array7,3) == 34);
    assert(middle_low_calf(array8,3) == 36);
    assert(middle_low_calf(array9,3) == 0);

    //TESTING HIGH_NORM && BELOW SIX
    int array10[3]={17,27,39};
    int array11[3]={15,16,27};
    int array12[3]={17,39,47};
    int array13[3]={16,39,47};
    int array14[4]={14,13,25,46};
    int array15[4]={14,24,44,34};
    int array16[4]={10,20,30,40};
    int array17[4]={10,33,35,47};
    int array18[3]={17,28,29};
    int array19[4]={10,21,30,40};
    int array20[4]={15,24,37,46};
    assert(high_norm(array10,3) == 17);
    assert(high_norm(array11,3) == 27);
    assert(high_norm(array12,3) == 17);
    assert(high_norm(array13,3) == 16);
    assert(high_norm(array14,4) == 46);
    assert(high_norm(array15,4) == 14);
    assert(high_norm(array16,4) == 10);
    assert(high_norm(array17,4) == 10);
    assert(high_norm(array18,3) == 29);
    assert(high_norm(array19,4) == 21);
    assert(high_norm(array20,4) == 46);

    //TESTING HAVE_SAME_FIRST_DIGIT
    int current[1] = {10};
    int array21[3] = {11,23,24};
    int array22[3] = {11,25,31};
    int array23[3] = {11,21,31};
    int array24[3] = {24,25,36};
    assert(have_same_first_digit(array21,3,current) == TRUE);
    assert(have_same_first_digit(array22,3,current) == TRUE);
    assert(have_same_first_digit(array23,3,current) == TRUE);
    assert(have_same_first_digit(array24,3,current) == FALSE);

   //TESTING ROUND_HAS_PENALTY
    int current1[3] = {25,26,31};
    int current2[3] = {25,27,39};
    int current3[3] = {26,31,47};
    int current4[3] = {11,12,47};
    int current5[3] = {13,22,49};
    assert(round_has_penalty(current1,3) == TRUE);
    assert(round_has_penalty(current2,3) == TRUE);
    assert(round_has_penalty(current3,3) == TRUE);
    assert(round_has_penalty(current4,3) == TRUE);
    assert(round_has_penalty(current5,3) == FALSE);

    //TESTING HIGHEST_UNDER_KNOWN 
    int current6[1] = {47};
    int current7[1] = {44};
    int current8[2] = {43,47};
    int current9[3] = {44,29,47};

    int array25[3] = {44,45,46};
    int array26[3] = {26,44,45};
    int array27[3] = {46,48,49};
    int array28[3] = {42,45,48};

    int array29[3] = {40,42,43};
    int array30[3] = {42,46,47};
    int array31[3] = {42,43,46};
    int array32[3] = {26,31,42};
    assert(highest_under_known(array25,3,current6,1) == 46);
    assert(highest_under_known(array26,3,current6,1) == 45);
    assert(highest_under_known(array27,3,current6,1) == 46);
    assert(highest_under_known(array28,3,current6,1) == 45);

    assert(highest_under_known(array29,3,current7,1) == 43);
    assert(highest_under_known(array30,3,current7,1) == 42);
    assert(highest_under_known(array31,3,current7,1) == 43);
    assert(highest_under_known(array32,3,current7,1) == 42);

    assert(highest_under_known(array25,3,current8,2) == 46);
    assert(highest_under_known(array26,3,current8,2) == 45);
    assert(highest_under_known(array27,3,current8,2) == 46);
    assert(highest_under_known(array28,3,current8,2) == 45);

    assert(highest_under_known(array25,3,current9,3) == 46);
    assert(highest_under_known(array26,3,current9,3) == 45);
    assert(highest_under_known(array27,3,current9,3) == 46);
    assert(highest_under_known(array28,3,current9,3) == 45);

    //TEST HIGHEST 
    int current10[1] = {25};
    int array33[3] = {27,28,29};
    int array34[3] = {23,24,29};
    int array35[3] = {21,22,23};
    int array36[3] = {19,27,38};
    int array37[3] = {18,19,26};
    assert(highest(array33,3,current10) == 29);
    assert(highest(array34,3,current10) == 29);
    assert(highest(array35,3,current10) == 23);
    assert(highest(array36,3,current10) == 27);
    assert(highest(array37,3,current10) == 26);

    //TEST HIGHEST BUFFALO
    int current11[1] = {45};
    int array38[3] = {31,44,46};
    int array39[3] = {23,24,40};
    int array40[3] = {46,48,49};
    int array41[3] = {42,43,49};
    int array42[3] = {32,40,47};
    int array43[3] = {44,45,46};
    assert(highest_buffalo(array38,3,current11) == 46);
    assert(highest_buffalo(array39,3,current11) == 40);
    assert(highest_buffalo(array40,3,current11) == 46);
    assert(highest_buffalo(array41,3,current11) == 43);
    assert(highest_buffalo(array42,3,current11) == 40);
    assert(highest_buffalo(array43,3,current11) == 46);

    //TEST ANY CARD
    int array44[3] = {46,48,49};
    int array45[4] = {37,38,42,49};
    int array46[3] = {36,37,38};
    int array47[3] = {20,37,38};
    int array48[3] = {21,31,32};
    int array49[4] = {17,18,21,29}; 
    assert(any_card(array44,3) == 48);
    assert(any_card(array45,4) == 49);
    assert(any_card(array46,3) == 38);
    assert(any_card(array47,3) == 38);
    assert(any_card(array48,3) == 21);
    assert(any_card(array49,4) == 29);

    //TEST LAST
    int array50[3] = {27,36,49};
    int array51[4] = {21,27,34,35};
    assert(last(array50,3) == 36);
    assert(last(array51,4) == 21);
  
    //TEST LAST NORM
    assert(last_norm(array50,3) == 27);
    assert(last_norm(array51,4) == 21);
 
    //TEST LOWEST WITH DIGIT
    int current12[1] = {26};
    int array52[3] = {21,25,27};
    int array53[3] = {24,31,42};
    int array54[3] = {24,27,31};
    assert(lowest_with_digit(array52,3,current12) == 21);
    assert(lowest_with_digit(array53,3,current12) == 24);
    assert(lowest_with_digit(array54,3,current12) == 24);

    //TEST CARD APPEARS
    int array55[3] = {21,25,36};
    assert(card_appears(36,array55,3) == TRUE);
    assert(card_appears(20,array55,3) == FALSE);
 
    //TEST VOIDS
    int array56[3] = {36,45,47};
    int array57[4] = {21,27,34,35};
    assert(voids(3,array56) == 36);
    assert(voids(4,array57) == 0);

    //TEST BUFFALO
    int array100[3] = {47,48,49};
    int array58[3] = {37,42,47};
    int array59[3] = {17,27,37};
    int array60[3] = {42,47,48};
    assert(buffalo(array100,3) == TRUE);
    assert(buffalo(array58,3) == TRUE);
    assert(buffalo(array59,3) == FALSE);
    assert(buffalo(array60,3) == TRUE);

    //TEST CARD BUFFALO HIGHER 
    assert(card_higher_buffalo(array100,3) == TRUE);
    assert(card_higher_buffalo(array58,3) == FALSE);
    assert(card_higher_buffalo(array59,3) == FALSE);
    assert(card_higher_buffalo(array60,3) == TRUE);

    //TEST HIGHEST EXCLUDE BUFFALO
    int array61[3] = {47,48,49};
    int array62[3] = {42,43,47};
    int array63[3] = {42,43,45};
    assert(highest_exclude_buffalo(array61,3) == 49);
    assert(highest_exclude_buffalo(array62,3) == 43);
    assert(highest_exclude_buffalo(array63,3) == 45);

    //TEST  HIGHER BUFFALO
    int array64[3] = {47,48,49};
    int array65[3] = {37,39,48};
    int array66[3] = {34,46,47};
    assert(buffalo_higher(array64,3) == 49);
    assert(buffalo_higher(array65,3) == 48);
    assert(buffalo_higher(array66,3) == 0);

   

    //TEST STRATEGY 
    //first to play in round -> not first round
    int current16[1] = {0};
    int play_card = 0;
    int prev5[4] = {10,11,12,13};
    int prev6[4] = {10,11,12,31};
    int prev7[8] = {10,11,12,20,21,22,23,31};

    //->calf played
    int array101[9] = {15,16,20,21,27,33,36,38,39};
    assert(strategy(array101,9,prev6,4,current16,0) == 20);
    play_card = 0;
    not_first_round(array101,9,prev6,4,current16,0,&play_card);
    assert(play_card == 20);

    int array102[8] = {16,17,24,25,32,33,34,35};
    assert(strategy(array102,8,prev7,8,current16,0) == 32);
    play_card = 0;
    not_first_round(array102,8,prev7,8,current16,0,&play_card);
    assert(play_card == 32);

    //->calf not played ->only has calf cards ->only one card
    int array103[1] = {31};
    int prev8[36] = {10};
    assert(strategy(array103,1,prev8,36,current16,0) == 31);
    play_card = 0;
    no_calves_prev(array103,1,prev8,36,current16,0,&play_card);
    assert(play_card == 31);

    //->more than one card
    int array104[9] = {30,31,32,33,34,35,36,37,38};
    assert(strategy(array104,9,prev5,4,current16,0) == 36);
    play_card = 0;
    no_calves_prev(array104,9,prev5,4,current16,0,&play_card);
    assert(play_card == 36);

   //-> not only has calf cards
    int array105[9] = {15,16,23,24,25,26,34,35,44};
    assert(strategy(array105,9,prev5,4,current16,0) == 23);
    play_card = 0;
    no_calves_prev(array105,9,prev5,4,current16,0,&play_card);
    assert(play_card == 23);
 
    //first to play in round -> first round
    // -> only calf cards
    int array106[10] = {30,31,32,33,34,35,36,37,38,39};
    assert(strategy(array106,10,prev5,0,current16,0) == 36);
    play_card = 0;
    first_round(array106,10,prev5,0,current16,0,&play_card);
    assert(play_card == 36);

    //-> not only calf cards -> has buffalo and higher
    int array107[10] = {10,11,12,13,14,23,24,25,47,48};
    assert(strategy(array107,10,prev5,0,current16,0) == 48);
    play_card = 0;
    first_round(array107,10,prev5,0,current16,0,&play_card);
    assert(play_card == 48);

    // -> no buffalo, has higher
    //void
    int array108[10] = {13,14,15,16,17,28,34,35,46,48};
    assert(strategy(array108,10,prev5,0,current16,0) == 28);
    play_card = 0;
    first_round(array108,10,prev5,0,current16,0,&play_card);
    assert(play_card == 28);

    //no void
    int array109[10] = {12,13,15,23,29,35,36,45,46,48};
    assert(strategy(array109,10,prev5,0,current16,0) == 29);
    play_card = 0;
    first_round(array109,10,prev5,0,current16,0,&play_card);
    assert(play_card == 29);

    //no higher, has buffalo
    //void
    int array110[10] = {11,12,13,14,15,16,27,45,46,47};
    assert(strategy(array110,10,prev5,0,current16,0) == 27);
    play_card = 0;
    first_round(array110,10,prev5,0,current16,0,&play_card);
    assert(play_card == 27);

    //no void
    int array111[10] = {12,14,15,16,26,29,32,33,46,47};
    assert(strategy(array111,10,prev5,0,current16,0) == 29);
    play_card = 0;
    first_round(array111,10,prev5,0,current16,0,&play_card);
    assert(play_card == 29);

    //void but it's a buffalo
    int array112[10] = {23,24,25,26,28,30,31,32,33,47};
    assert(strategy(array112,10,prev5,0,current16,0) == 26);
    play_card = 0;
    first_round(array112,10,prev5,0,current16,0,&play_card);
    assert(play_card == 26);

    //not first to play in round -> has same digit
    int current17[2] = {25,32};
    
    int array113[10] = {14,15,16,23,24,34,35,36,44,45};
    assert(strategy(array113,10,prev5,0,current17,2) == 24);
    play_card = 0;
    play_same_digit(array113,10,prev5,0,current17,2,&play_card);
    assert(play_card == 24);

    int array114[10] = {14,15,16,26,27,34,35,36,44,45};
    assert(strategy(array114,10,prev5,0,current17,2) == 26);
    play_card = 0;
    play_same_digit(array114,10,prev5,0,current17,2,&play_card);
    assert(play_card == 26);

    // -> last to play
    int current18[3] = {42,43,46};
     
    int array115[10] = {20,21,22,23,30,31,32,44,47,49};
    assert(strategy(array115,10,prev5,0,current18,3) == 49);
    play_card = 0;
    last_to_play(array115,10,prev5,0,current18,3,&play_card);
    assert(play_card == 49);
 
    int array116[10] = {20,21,22,23,30,31,32,41,44,49};
    assert(strategy(array116,10,prev5,0,current18,3) == 49);
    play_card = 0;
    last_to_play(array116,10,prev5,0,current18,3,&play_card);
    assert(play_card == 49);
    
    int current19[3] = {24,25,27};
    int array117[10] = {22,23,28,31,32,34,41,42,43,44};
    assert(strategy(array117,10,prev5,0,current19,3) == 28);
    play_card = 0;
    last_to_play(array117,10,prev5,0,current19,3,&play_card);
    assert(play_card == 28);



    // -> not last to play
    int current14[1] = {45};
    int array71[3] = {43,47,49};
    int array72[9] = {10,11,12,13,14,15,43,46,49};
    int array73[9] = {10,11,12,13,14,15,43,46,47};

    assert(strategy(array71,3,prev5,4,current14,1) == 43);
    play_card = 0;
    not_last_to_play(array71,3,prev5,4,current14,1,&play_card);
    assert(play_card == 43);

    assert(strategy(array72,9,prev5,4,current14,1) == 46);
    play_card = 0;
    not_last_to_play(array72,9,prev5,4,current14,1,&play_card);
    assert(play_card == 46);

    assert(strategy(array73,9,prev5,4,current14,1) == 46);
    play_card = 0;
    not_last_to_play(array73,9,prev5,4,current14,1,&play_card);
    assert(play_card == 46);


    int current15[1] = {25};
    int array74[3] = {26,27,28};
    int array75[3] = {22,23,24};

    assert(strategy(array74,3,prev5,4,current15,1) == 26);
    play_card = 0;
    not_last_to_play(array74,3,prev5,4,current15,1,&play_card);
    assert(play_card == 26);

    assert(strategy(array75,3,prev5,4,current15,1) == 24);
    play_card = 0;
    not_last_to_play(array75,3,prev5,4,current15,1,&play_card);
    assert(play_card == 24);

    //not first to play -> no same digit -> has buffalo
    int array118[10] = {10,11,12,34,35,36,37,38,39,47};
    assert(strategy(array118,10,prev5,0,current19,3) == 47);

  

}


// ADD YOUR FUNCTIONS HERE
void scan_cards(int n_cards, int hand[]){
    int i = 0;
    while (i < n_cards){
	scanf("%d", &hand[i]);
	i++;
    }
}

int last_digit(int num){
    int digit;
    digit = num % 10;
    return digit;
}

int first_digit (int num){
    int digit;
    digit = num / 10;
    return digit;
}

//returns number of cards which start with the same number
int n_tens(int n_cards, int hand[], int tens){
    int num = 0;
    int i = 0;
    while (i < n_cards){
	if (first_digit(hand[i]) == tens){
	    num++;
	}
	i++;
    }
    return num;
}

//discarding cards higher than buffalo
void discard_high_buffalo(int cards[], int discard[], int *i, int *n){
    while (first_digit(cards[*n]) == 4){
	if(cards[*n] >= BUFFALO){
	    discard[*i] = cards[*n];
	    cards[*n]= DISCARDED;
	    *i = *i + 1;
	}
    *n = *n - 1;
    }
}

//discarding high calves (35 and up)
void discard_high_calves(int cards[],int discard[], int *i, int *n){
    while(first_digit(cards[*n]) >= 3 && *i < 3){
	//making sure there's still empty elements in discard array
	if(last_digit(cards[*n]) >= 5){
	    discard[*i] = cards[*n];
	    cards[*n] = DISCARDED;
	    *i = *i + 1;
	}
	*n = *n - 1;
    }
}

//discarding any card with last digit higher than 5 
//(except when it's the only calf)
int discard_high(int hand[], int digit, int i, int discard[]){
    int l = N_CARDS_INITIAL_HAND - 1;
     	while(l>= 0 && i < 3){
	//making sure there's still empty elements in discard array
	    if(last_digit(hand[l]) == digit){
		if (first_digit(hand[l])!= 3 ||
		    n_tens(N_CARDS_INITIAL_HAND,hand,3) > 1){
		    discard[i] = hand[l];
		    hand[l] = DISCARDED;
		    i++;
		}
	    }
	    l--;
	}
    return i;
}

//discarding highest last digit non-calf cards (last resort)
int discard_highest_norm(int hand[], int digit, int i, int discard[]){
    int l = 0;
     	while(l < N_CARDS_INITIAL_HAND && i < 3){
	//making sure there's still empty elements in discard array
	    if(last_digit(hand[l]) == digit && first_digit(hand[l]) <=2){
		    discard[i] = hand[l];
		    hand[l] = DISCARDED;
		    i++;
	    }
	    l++;
	}
    return i;
}

//returns a card to play 
int strategy(int cards_mine[], int n_cards_mine, int prev_played[],
	     int n_prev_played, int current_round[], int n_current_round){

    int play_card = NOT_CHOSEN;
    if (n_current_round == 0){ //first to play in any round
	if (n_prev_played > 0){ //not first round
	    not_first_round(cards_mine, n_cards_mine, prev_played, 
			    n_prev_played, current_round, n_current_round, 
			    &play_card);
	} else {//first round
   	    first_round(cards_mine, n_cards_mine, prev_played, n_prev_played,
			current_round, n_current_round, &play_card);
  	}
    } else {//not first to play in a round
	if (have_same_first_digit(cards_mine, n_cards_mine, current_round) == 
	    TRUE){
	    play_same_digit(cards_mine, n_cards_mine, prev_played, 
			    n_prev_played, current_round, n_current_round, 
			    &play_card);
	} else {//none of my cards start with same digit as card played in round
	    if (buffalo(cards_mine,n_cards_mine) == TRUE){
		play_card = BUFFALO;
	    }
	    if (play_card == NOT_CHOSEN){
	        play_card = any_card(cards_mine, n_cards_mine);
	    }
	}
    }
    return play_card;
}

//chooses a card when it is not the first round
void not_first_round(int cards_mine[], int n_cards_mine, int prev_played[],
		     int n_prev_played, int current_round[], 
		     int n_current_round, int *play_card){

    if (calf_prev_played(prev_played,n_prev_played) == TRUE){
	   *play_card = last(cards_mine, n_cards_mine);
    } else {//not first round and no calves played yet
        no_calves_prev(cards_mine, n_cards_mine, prev_played, n_prev_played, 
		       current_round, n_current_round, play_card);
    }
}

//chooses card when calves are illegal 
void no_calves_prev(int cards_mine[], int n_cards_mine, int prev_played[],
		    int n_prev_played, int current_round[], int n_current_round, 
		    int *play_card){

    if (only_calf_cards(cards_mine,n_cards_mine) == TRUE){
	if (n_cards_mine == 1){ //only have one card
	    *play_card = cards_mine[0];
        } else {
	    *play_card = middle_low_calf(cards_mine, n_cards_mine);
 	}
    } else {
	//lowest card (except calf), last measure
 	    *play_card = last_norm(cards_mine, n_cards_mine);
        
    }
}

//chooses card when player is in first round
void first_round(int cards_mine[], int n_cards_mine, int prev_played[],
		 int n_prev_played, int current_round[], int n_current_round, 
		 int *play_card){

    if (only_calf_cards(cards_mine, n_cards_mine) == TRUE){
	if (n_cards_mine == 1){
	    *play_card = cards_mine[0];
	} else {
	    *play_card = middle_low_calf(cards_mine, n_cards_mine);
    	    if(*play_card == NOT_CHOSEN){
		*play_card = cards_mine[0];
	    }
 	}
    } else { 
	//otherwise
	if (*play_card == NOT_CHOSEN){ //create voids
	    *play_card = voids(n_cards_mine, cards_mine);
	}
        if (*play_card >= BUFFALO || first_digit(*play_card) == 3){
	//cannot play calf and don't want to play higher than buffalos
	    *play_card = NOT_CHOSEN;
        }
	//when player has buffalo and higher, playing a higher card has no risk
	if (buffalo(cards_mine, n_cards_mine) == TRUE && 
	    card_higher_buffalo(cards_mine, n_cards_mine) == TRUE){
	    *play_card = buffalo_higher(cards_mine, n_cards_mine);
	} 
	if(*play_card == NOT_CHOSEN){ //play highest 10s 20s 40s (under buff)

 	    *play_card = high_norm(cards_mine,n_cards_mine);
	}
    }
}

//chooses card when player has cards with same first digit as cards in round
void play_same_digit(int cards_mine[], int n_cards_mine, int prev_played[],
		     int n_prev_played, int current_round[], 
		     int n_current_round, int *play_card){

    if (round_has_penalty(current_round, n_current_round) == TRUE){
	*play_card = highest_under_known(cards_mine, n_cards_mine, 
					current_round, n_current_round);
	if (*play_card == NOT_CHOSEN){
	//if no cards are lower than already played cards
	    *play_card = lowest_with_digit(cards_mine, n_cards_mine, 
					   current_round);
	}
    } else { //no penalty
	if (n_current_round == 3){ 
	    last_to_play(cards_mine, n_cards_mine, prev_played, n_prev_played,
			 current_round, n_current_round, play_card);
	} else {
	    not_last_to_play(cards_mine, n_cards_mine, prev_played, 
			     n_prev_played, current_round, n_current_round,
			     play_card);
   	}
    }
}

//chooses card when player is the last to play in current round
void last_to_play(int cards_mine[], int n_cards_mine, int prev_played[],
		  int n_prev_played, int current_round[], int n_current_round, 
		  int *play_card){

    if(first_digit(current_round[0]) == 4 && 
       buffalo(cards_mine,n_cards_mine) == TRUE){
    //player holds buffalo card   
	*play_card = highest_exclude_buffalo(cards_mine,n_cards_mine);
    } 
    if (*play_card == NOT_CHOSEN){ 
    //cards in round do not start with 4
	*play_card = highest(cards_mine, n_cards_mine, current_round);
    }  
}

//chooses card when player is not the last to play in current round
void not_last_to_play(int cards_mine[],int n_cards_mine,int prev_played[],
		      int n_prev_played,int current_round[],
		      int n_current_round, int *play_card){

    if (n_cards_mine >= 8){ //1,2,3 rounds
    //there are cards which are higher that have not been played yet
	if (first_digit(current_round[0]) == 4 ){
	    consider_buffalo(cards_mine, n_cards_mine, prev_played,
			     n_prev_played, current_round, n_current_round,
			     play_card);
	}
	if (*play_card == NOT_CHOSEN && current_round[0] != 3){
	    *play_card = highest(cards_mine, n_cards_mine, current_round);
	}
    } else {
	*play_card = highest_under_known(cards_mine, n_cards_mine, 
					current_round, n_current_round);
	if (*play_card == NOT_CHOSEN){//no cards are higher than already played
	    *play_card = lowest_with_digit(cards_mine, n_cards_mine,
					   current_round);
	}
    }
}

void consider_buffalo(int cards_mine[],int n_cards_mine,int prev_played[],
		      int n_prev_played,int current_round[],
		      int n_current_round, int *play_card){
	
    if (buffalo(cards_mine,n_cards_mine) == TRUE &&
	card_higher_buffalo(cards_mine,n_cards_mine) == TRUE){
    //player has both the buffalo and a card higher
   	*play_card = buffalo_higher(cards_mine, n_cards_mine); 
    //card higher is no longer dangerous
    }
    if (*play_card == NOT_CHOSEN){
	 *play_card = highest_buffalo(cards_mine, n_cards_mine, current_round);
    }
}

//determines if a calf has already been played 
int calf_prev_played(int prev_played[],int n_prev_played){
    int result = FALSE;
	int i = 0;
	while (i < n_prev_played){
	    if (first_digit(prev_played[i]) == 3){
		result = TRUE;
	    }
	    i++;
	}
    return result;
}

//determines if player only has calf cards
int only_calf_cards (int cards_mine[],int n_cards_mine){
    int result = TRUE;
    int i = 0;
    while (i < n_cards_mine){
	if (first_digit(cards_mine[i]) != 3){
	    result = FALSE;
	}
	i++;
    }
    return result;

}
    
//finds the highest calf card with last digit less than/equal to 6
int middle_low_calf(int cards_mine[],int n_cards_mine){
    int play_card = NOT_CHOSEN;
    int digit = 6;
    int i  = 0;
    while (digit >= 0 && play_card == 0){
	i = 0;
	while (i < n_cards_mine){
	    if (last_digit(cards_mine[i]) == digit){
	        play_card = cards_mine[i];
		break; //stops find a card when card has been chosen
	    }
	    i++;
	}
	digit--;
    }
    return play_card;

}

//finds the highest normal (non-calf,lower than buffalo) card 
int high_norm(int cards_mine[], int n_cards_mine){
    int play_card = NOT_CHOSEN;
    int i = 0;
    int digit = 9;
    //considers high 10s and 20s w/ last digit above 7
    while (digit >= 7 && play_card == NOT_CHOSEN){
	i = 0;
	while (first_digit(cards_mine[i]) <= 2 && i < n_cards_mine){
	    if (last_digit(cards_mine[i]) == digit &&
                n_tens(n_cards_mine,cards_mine,first_digit(cards_mine[i])) < 4){
		play_card = cards_mine[i];
		break;//stops finding card when a card has been chosen
	    }
	    i++;
  	    if ( i == n_cards_mine){
		break;
   	    }
	}
	digit--;
    }

    digit = 6;
    if (play_card ==NOT_CHOSEN){
	play_card = below_six(cards_mine, digit, n_cards_mine);
    }
    return play_card;
}
    
//chooses highest normal card (non-calf,below buffalo) under six
int below_six(int cards_mine[], int digit, int n_cards_mine){
    int play_card = NOT_CHOSEN;
    int i = 0;
    while (digit >= 0 && play_card == NOT_CHOSEN){
    	i = 0;
	while (i < n_cards_mine && play_card == NOT_CHOSEN){
	    if (last_digit(cards_mine[i]) == digit && 
		first_digit(cards_mine[i]) != 3){
		play_card = cards_mine[i];
		break;//stops finding card when a card has been chosen
	    }
	    i++;
	}
	digit--;
    }
    return play_card;
}	
    
//determines if player has cards that start with same digit as cards in round
int have_same_first_digit(int cards_mine[], int n_cards_mine,
			  int current_round[]){
    int result = FALSE;
    int i = 0;
    while ( i< n_cards_mine){
	if (first_digit(cards_mine[i]) == first_digit(current_round[0])){
	    result = TRUE;
	}
	i++;
    }
    return result;
}

//determines if round contains any calves or buffalo
int round_has_penalty(int current_round[], int n_current_round){
    int penalty = FALSE;
    int i = 0;
    while (i < n_current_round){
	if (first_digit(current_round[i]) == 3 || current_round[i] == BUFFALO){
	    penalty = TRUE;
	}
	i++;
    }
    return penalty;
}

//finds the highest card lower than highest cards in round
int highest_under_known(int cards_mine[], int n_cards_mine,
		        int current_round[], int n_current_round){

    int play_card = NOT_CHOSEN;
    int highest_known = current_round[0];
    int digit = first_digit(current_round[0]);
    //find highest card played in round starting with same digit as first card
    //in round
    int i = 1;
    while (i < n_current_round){
	if (current_round[i] > highest_known &&
	    first_digit(current_round[i]) == digit){
	    highest_known = current_round[i];
	}
	i++;
    }
    //find highest card in hand that is lower than highest_known
    i = 0;
    while (i < n_cards_mine){
	if (first_digit(cards_mine[i]) == digit && 
            last_digit(cards_mine[i]) < last_digit(highest_known)){
	    play_card = cards_mine[i];
	}
	i++;
    }
    return play_card;
}

//finds the highest card 
int highest(int cards_mine[], int n_cards_mine, int current_round[]){
    int play_card = NOT_CHOSEN;
    int digit = first_digit(current_round[0]);
    int i = 0;
    while ( i < n_cards_mine){
	if (first_digit(cards_mine[i]) == digit){
	    play_card = cards_mine[i];
	}
	i++;
    }
    return play_card;
}

//finds the highest card that is still lower than buffalo
int highest_buffalo(int cards_mine[], int n_cards_mine, int current_round[]){
    int play_card = NOT_CHOSEN;
    int digit = first_digit(current_round[0]);
    int i = 0;
    while ( i < n_cards_mine){
	if (first_digit(cards_mine[i]) == digit && 
	    last_digit(cards_mine[i]) < 7){
	    play_card = cards_mine[i];
	}
	i++;
    }
    return play_card;
}

//strategy to discard the most risky cards
int any_card(int cards_mine[], int n_cards_mine){
    int play_card = 0;
    int i = 0;
    //any cards higher than buffalo
    while ( i < n_cards_mine && play_card == NOT_CHOSEN){
	if (cards_mine[i] >= BUFFALO){
	    play_card = cards_mine[i];
	}
	i++;
    }
    //34,35,36,37,38,39 cards 
    i = n_cards_mine - 1; 
    while ( i >= 0 && play_card == NOT_CHOSEN){
	if (cards_mine[i] >= 34 && cards_mine[i] <= 39){
	    play_card = cards_mine[i];
	}
	i--;
    }
    //creating voids
    if (play_card == NOT_CHOSEN){
        play_card = voids(n_cards_mine,cards_mine);
    }
    //cycles through rest of the cards ,discards highest
    int digit = 9;
    while (digit >= 0 && play_card == NOT_CHOSEN){
        i = 0;
	while (i < n_cards_mine && play_card == NOT_CHOSEN){
	    if (last_digit(cards_mine[i]) == digit){
		play_card = cards_mine[i];
	    }
	    i++;
	}
	digit--;
    }
    return play_card;
}


//finds the lowest card,last measure
int last(int cards_mine[], int n_cards_mine){
    int play_card = NOT_CHOSEN;
    int i = n_cards_mine - 1;
    int digit = 0;
    while (digit <= 9 && play_card == NOT_CHOSEN){
	i = n_cards_mine - 1;
        while ( i >= 0 && play_card == NOT_CHOSEN ){
	    if (last_digit(cards_mine[i]) == digit){
		play_card = cards_mine[i];
	    }
	    i--;
        }
	digit++;
    }
    return play_card;
}

//determines if specific card is in a hand
int card_appears(int card, int hand[], int n_hand){
    int appears = FALSE;
    int i = 0;
    while ( i < n_hand){
	if (hand[i] == card){
	    appears = TRUE;
    	}
	i++;
    }
    return appears;
}

//finds cards when they are the last in their 10s group, to create voids
int voids(int n_cards_mine, int cards_mine[]){
    int play_card = NOT_CHOSEN;
    int i = 1;
    int digit = 0;
    int voids = FALSE;
   //finding single cards
    while (i <= 4){
	if (n_tens(n_cards_mine,cards_mine,i) == 1){
	    digit = i;
	    voids = TRUE;
	}
	i++;
    }
    //if there are single cards, play them
    if (voids == TRUE && play_card == NOT_CHOSEN){
        i = 0;
	while ( i < n_cards_mine && play_card == NOT_CHOSEN){
	    if (first_digit(cards_mine[i]) == digit){
		play_card = cards_mine[i];
	    }
	    i++;
	}
    }
    return play_card;
}


//finds lowest normal card, last measure
int last_norm(int cards_mine[], int n_cards_mine){

    int play_card = NOT_CHOSEN;
    int i = n_cards_mine - 1;
    int digit = 0;
    while (digit <= 9 && play_card == NOT_CHOSEN){
	i = n_cards_mine - 1;
        while ( i >= 0 && play_card == NOT_CHOSEN){
	    if (last_digit(cards_mine[i]) == digit && 
		first_digit(cards_mine[i]) != 3){
		//exclude calves
		play_card = cards_mine[i];
	    }
	    i--;
        }
	digit++;
    }
    return play_card;
}

//chooses lowest card given the first digit 
int lowest_with_digit(int cards_mine[],int n_cards_mine,int current_round[]){
    int play_card = NOT_CHOSEN;
    int digit = first_digit(current_round[0]);
    int i = 0;
    while ( i < n_cards_mine && play_card == NOT_CHOSEN){
	if (first_digit(cards_mine[i]) == digit){
	    play_card = cards_mine[i];
	}
	i++;
    }
    return play_card;

}

//determines if player holds buffalo
int buffalo(int cards_mine[], int n_cards_mine){
    int result = FALSE;
    int i = 0;
    while ( i < n_cards_mine){
	if (cards_mine[i] == BUFFALO){
	   result = TRUE;
	}
	i++;
    }
    return result;
}
//plays the card that is higher than buffalo
int buffalo_higher(int cards_mine[], int n_cards_mine){
    int card = 0;
    int i = n_cards_mine - 1;
    while ( i >= 0 && card == 0){
	if (cards_mine[i] > BUFFALO){
	   card = cards_mine[i];
	}
	i--;
    }
    return card;
}

//finds the highest card excluding the buffalo
int highest_exclude_buffalo(int cards_mine[] ,int n_cards_mine){
    int play_card = NOT_CHOSEN;
    int i = n_cards_mine - 1;
    while ( i>=0 && play_card == NOT_CHOSEN){
	if (first_digit(cards_mine[i]) == 4 && cards_mine[i] != BUFFALO){
	    play_card = cards_mine[i];
	}
	i--;
    }
    return play_card;	
}

//determines if player has cards higher than buffalo
int card_higher_buffalo(int cards_mine[],int n_cards_mine){
    int card = FALSE;
    int i = 0;
    while ( i < n_cards_mine){
	if ( cards_mine[i] > BUFFALO){
	    card = TRUE;
	}
	i++;
    }
    return card;
}



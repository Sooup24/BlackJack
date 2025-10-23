#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct playerInfo
{
    // stuff for gameplay loop here?
    int handTotal;
    int hasAce;
    bool aceFlipped;
} player;

int main()
{
    // =-=-=-=-=-=-=-= Conditions and setup =-=-=-=-=-=-=-=-=
    int decks = 2;
    int cards[11];
    int myStrategy = 15;
    int numPlayers = 4;    // includes me, so 3 players would be 2 bots+me (Changed to include dealer in this as well)
    //maybe want to include dealer in numPlayers because of how we track aces in hands?
    int table[numPlayers]; // How seating done at the table 
    player hands[numPlayers];
    int mySeat = 1;        // which seat are you at table?
    srand(time(NULL));

    int generations = 1; // How many games to simulate?

    // fill seats at table
    for (int i = 0; i < numPlayers; i++)
    {
        hands[i].handTotal = 0;
        hands[i].hasAce = 0;
        hands[i].aceFlipped = false;
        if (i+1 == mySeat)
            table[i] = myStrategy; // swapped logic so last spot can always be dealer
        else
            table[i] = rand() % 7 + 12; // set bot strategy, 12-18
    }
    table[numPlayers-1] = 15; // this is the dealer stratagy   // changed == 15 to = 15

    bool turn = true;
    int win = 0, loss = 0, draw = 0;
    
    //=-=-=-=--=-= GAME SIMULATION =-=-=--=-=-=-=-=-
    for (int i = 0; i < generations; i++)
    {
        // create playing deck
        for (int k = 1; k < 11; k++) // skip index 0, each index will correspond to card num/value
            if (k == 10){
                cards[i] = 16 * decks;
            } else {
                cards[i] = 4 * decks;
            }
        // deal 2 cards to each player INITIAL DEAL
        int card;
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < numPlayers; j++)
            {
                // pass out a card, calculate handTotal
                card = rand() % 10 + 1;
                if (card == 1){
                    hands[j].hasAce += 1;   // changed from true
                    hands[j].handTotal += 11;
                    //printf("Player %d given Ace!\n", j);
                } else {
                    hands[j].handTotal += card;
                    //printf("Player %d given %d\n", j, card);
                }

                cards[card]--;
            }
        }
        // for (int k = 0; k < numPlayers; k++){
        //     printf("Player %d - Hand Total: %d\n", k, hands[k].handTotal);
        // }

        for (int j = 0; j < numPlayers; j++) // loop thru each player TURN
        {
            while (turn == true)
            {
                // Logic for aces and card values I think will go here ===============================

                if (hands[j].handTotal < table[j]) // hit
                {
                    //  deal another card
                }
                else if (hands[j].handTotal >= table[j])
                    turn = false;
                else if (hands[j].handTotal > 21) // player busts
                    break;
            }
            turn = true;
        }
        // All players have gone, dealer plays, calc new win / loss / tie %

        // Dealer play here ================================

        // Only need to care about player hand, not the bots ============================
        // Need a way to handle this still, maybe an arr?
        if (hands[mySeat].handTotal > hands[numPlayers].handTotal && hands[mySeat].handTotal <= 21)
            win++;
        else if (hands[numPlayers].handTotal > 21)
            win++;
        else if (hands[mySeat].handTotal > 21)
            loss++;
        else if (hands[mySeat].handTotal < hands[numPlayers].handTotal && hands[numPlayers].handTotal <= 21)
            loss++;
        else if (hands[mySeat].handTotal == hands[numPlayers].handTotal)
            draw++;
    }

    printf("With strategy hit under %d...\n", myStrategy);
    printf("Win: %d, Loss: %d, Draw: %d\n", win / generations, loss / generations, draw / generations);
}

//===========Gameplay loop for blackjack===========
// Dealer gives cards face up going clockwise (including self) until everyone has 2 cards
// real game, first card for dealer would be face down, but I think we can ignore for this
// Going clockwise, player either hits or stands
// if hit (can be multiple in one turn), deal another card to the player
// if stand go to next player
// Once all players had a turn, dealer would reveal their second card and hit until a number X

//==========WIN CONDITIONS=============
// Closer to 21 than dealer - win
// Dealer bust - win
// Tie hand with dealer - draw/wash, calcuate as a diff percentage
// Player bust - lose
// Player lower score than dealer - lose
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct playerInfo
{
    // stuff for gameplay loop here?
};

int main()
{
    // =-=-=-=-=-=-=-= Conditions and setup =-=-=-=-=-=-=-=-=
    int decks = 2;
    int cards[15];
    int myStrategy = 15;
    int dealerStrategy = 16;
    int numPlayers = 3;    // includes me, so 3 players would be 2 bots+me
    int table[numPlayers]; // How seating done at the table
    int mySeat = 1;        // which seat are you at table?
    srand(time(NULL));

    int generations = 10; // How many games to simulate?

    // fill seats at table
    for (int i = 1; i <= numPlayers; i++)
    {
        if (i == mySeat)
            table[i - 1] = myStrategy;
        else
            table[i - 1] = rand() % 7 + 12; // set bot strategy, 12-18
    }

    bool turn = true;
    int win = 0, loss = 0, draw = 0;
    int dealerTotal = 0;

    //=-=-=-=--=-= GAME SIMULATION =-=-=--=-=-=-=-=-
    for (int i = 0; i < generations; i++)
    {
        // create playing deck
        for (int k = 1; k < 15; k++) // skip index 0, each index will correspond to card num/value
            cards[i] = 4 * decks;
        // deal 2 cards to each player
        for (int k = 0; k < 2; k++)
            for (int j = 0; j < numPlayers; j++)
                // pass out a card, calculate handTotal, dealerTotal

                for (int j = 0; j < numPlayers; j++) // loop thru each player
                {
                    while (turn == true)
                    {
                        // Logic for aces and card values I think will go here ===============================

                        if (handTotal < table[j]) // hit
                        {
                            //  deal another card
                        }
                        else if (handTotal >= table[j])
                            turn = false;
                        else if (handTotal > 21) // player busts
                            break;
                    }
                    turn = true;
                }
        // All players have gone, dealer plays, calc new win / loss / tie %

        // Dealer play here ================================

        // Only need to care about player hand, not the bots ============================
        // Need a way to handle this still, maybe an arr?
        if (handTotal > dealerTotal && handTotal <= 21)
            win++;
        else if (dealerTotal > 21)
            win++;
        else if (handTotal > 21)
            loss++;
        else if (handTotal < dealerTotal && dealerTotal <= 21)
            loss++;
        else if (handTotal == dealerTotal)
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
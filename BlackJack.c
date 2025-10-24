#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>

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
    int decks = 4;
    int cards[11];
    int myStrategy = 16;
    int numPlayers = 4; // includes me, so 3 players would be 2 bots+me (Changed to include dealer in this as well)
    // maybe want to include dealer in numPlayers because of how we track aces in hands?
    int table[numPlayers]; // How seating done at the table
    player hands[numPlayers];
    int mySeat = 1; // which seat are you at table?
    int NUMT = 5;
    srand(time(NULL));

    int generations = 10; // How many games to simulate?

    // fill seats at table
    for (int i = 0; i < numPlayers; i++)
    {
        if (i + 1 == mySeat)
            table[i] = myStrategy; // swapped logic so last spot can always be dealer
        else
            table[i] = rand() % 7 + 12; // set bot strategy, 12-18
    }

    table[numPlayers - 1] = 16; // this is the dealer stratagy   // changed == 15 to = 15

    bool turn = true;
    float win = 0, loss = 0, draw = 0;

    //=-=-=-=--=-= GAME SIMULATION =-=-=--=-=-=-=-=-
    omp_set_num_threads(NUMT);
    #pragma omp parallel for schedule(dynamic, 1) reduction(+ : win, loss, draw) private(hands, cards, turn)
    for (int i = 0; i < generations; i++)
    {
        for (int i = 0; i < numPlayers; i++)
        {
            hands[i].handTotal = 0;
            hands[i].hasAce = 0;
            hands[i].aceFlipped = false;
        }

        // create playing deck
        for (int k = 1; k < 11; k++) // skip index 0, each index will correspond to card num/value
            if (k == 10)
            {
                cards[k] = 16 * decks;
            }
            else
            {
                cards[k] = 4 * decks;
            }
        // deal 2 cards to each player INITIAL DEAL
        int card;
        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < numPlayers; j++)
            {
                // pass out a card, calculate handTotal
                card = rand() % 10 + 1;
                if (card == 1)
                {
                    hands[j].hasAce += 1; // changed from true
                    if (hands[j].hasAce > 1)
                        hands[j].handTotal += 1;
                    else
                        hands[j].handTotal += 11;
                    // printf("Player %d given Ace!\n", j);
                }
                else
                {
                    hands[j].handTotal += card;
                    // printf("Player %d given %d\n", j, card);
                }

                cards[card]--;
            }
        }

        for (int j = 0; j < numPlayers; j++) // loop thru each player TURN
        {
            while (turn == true)
            {
                if (hands[j].handTotal < table[j]) // hit
                {
                    card = rand() % 10 + 1;
                    hands[j].handTotal += card;
                    if (hands[j].handTotal > 21 && hands[j].hasAce > 0 && hands[j].aceFlipped == false)
                    {
                        hands[j].handTotal -= 10;
                        hands[j].aceFlipped = true;
                    }
                }
                else if (hands[j].handTotal >= table[j]) // stand
                    turn = false;
                else if (hands[j].handTotal > 21) // player busts
                    turn = false;
                cards[card]--;

                // printf("card = %d   ", card);
                // printf("Current hand Player%d = %d\n", j, hands[j].handTotal);
            }
            turn = true;
            sleep(2.1);
        }
        // All players have gone, dealer plays, calc new win / loss / tie %

        for (int k = 0; k < numPlayers; k++)
                printf("Generation %d - Player %d - Hand Total: %d\n", i, k, hands[k].handTotal);

        if (hands[mySeat].handTotal > hands[numPlayers - 1].handTotal && hands[mySeat].handTotal <= 21)
        {
            win++;
            printf("Win\n");
        }
        else if (hands[numPlayers - 1].handTotal > 21)
        {
            win++;
            printf("Win\n");
        }
        else if (hands[mySeat].handTotal > 21)
        {
            loss++;
            printf("Loss\n");
        }
        else if (hands[mySeat].handTotal < hands[numPlayers - 1].handTotal && hands[numPlayers - 1].handTotal <= 21)
        {
            loss++;
            printf("Loss\n");
        }
        else if (hands[mySeat].handTotal == hands[numPlayers - 1].handTotal)
        {
            draw++;
            printf("Draw\n");
        }
        printf("\n");

    }

    printf("With strategy hit under %d...\n", myStrategy);
    printf("Win: %.0f%% , Loss: %.0f%%, Draw: %.0f%%\n", 100 * (win / generations), 100 * (loss / generations), 100 * (draw / generations));
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
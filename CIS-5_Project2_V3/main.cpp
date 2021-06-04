/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jakec
 * Project 1, Vesion 1: Baccarat
 * Created on May 9, 2021, 1:58 PM
 */

/*
 Rules to code from:
 *The dealer then deals out the cards face up—two each for 
  the player and banker—and whichever hand totals closest to 
  nine wins. If you’ve bet on the player hand and it has the 
  closest to nine, the winnings are simply double what you bet.

If you’ve bet on the banker hand and it wins, it pays 100 percent 
of your wager. When the cards dealt are greater than nine, 
you have to add the two together and drop the one (or two) 
to get the value.

For example, a hand of nine and seven cards dealt would add 
up to 16, and with the first digit dropped, the value in 
the game is six.

Here are some more rules to help you ace a baccarat game:

* If either the player or banker is dealt a total of eight or 
  nine, both the player and banker stand.
* If the player’s total is five or less, then the player will 
  receive another card. Otherwise, the player will stand.
* If the player stands, then the banker hits on a total of 5 
  or less.
* The final betting option, a tie, pays out 8-to-1. 
  Conveniently, there are also sheets at the table for 
  you to keep track of your score.
 
 */

//System Libraries
#include <iostream>    //Input/Output Library
#include <iomanip>     //Use for formatting and setting $ to 10ths place for cents
#include <stdlib.h>    //srand, rand
#include <time.h>      //time for random number seed purposes
#include <string>      //Displaying cards drawn, etc.
#include <fstream>     //To read and write statistics files
#include <ctime>       //Print out time
#include <cmath>       //Used for rounding to nearest $

using namespace std;   //Library Name-space

//User Libraries

//Global/Universal Constants -- No Global Variables
//Science, Math, Conversions, Higher Dimensioned constants only

//Function Prototypes
unsigned short randCrd(const int); //Function to randomize card #
unsigned short rndSuit(const int); //Function to randomize suit - # corresponds to suit
string suitGen(unsigned short suitVal[], const int n);
string faceGen(unsigned short cards[], const int n);

//Execution Begins Here
int main(int argc, char** argv) {
    //Set the Random number seed
    srand(time(NULL));
    //Declare variables
    const int crdSIZE = 6;
    const int paySIZE = 3;
    float house, //House Money (Cumulative)
          player, //Player money (Cumulative)
          betBank, //Bet on banker - 1:1
          betPlay, //Bet on player - 1:1
          betTie, //Bet on tie between banker + player; 8:1 payout
          pPair, //Bet on pair drawn for player; 11:1 payout
          bPair, //Bet on pair drawn for banker; 11:1 payout
          payOut[paySIZE], payTot,
          avgWin; //average winnings per round
    unsigned short bCards, //Banker cards drawn
                   pCards, //Player cards drawn
                   rounds, //rounds played (incremented)
                   cards[crdSIZE], //number on cards
                   suitVal[crdSIZE], //array of randomly generated numbers corresponding to suits
                   pTot, //Player total
                   bTot, //Banker total
                   subTot[crdSIZE]; //Used for calculating zero values for face cards 
    bool pBool,  //Player boolean to check if pair
         bBool; //Bank boolean for pair
    string suits[crdSIZE], //array of suits corresponding to drawn cards
           view[crdSIZE], //Face Card display stored in array
           statsLn; //Stats line from file
    char repeat; //y or n entered by user to indicate repeat
    fstream out, //For writing stats in file - output
            inBets; //File for taking in bets
    time_t curTime; //Current time
        
    //Initialize variables
    /*GAME PLAYED WITH 6-8 DECKS OF CARDS TO NO NEED TO MANIPULATE
      PROBABILITIES BASED UPON PAST DRAWS*/
    rounds = 0;

    inBets.open("inputBets.dat", ios::in); //File to take prewritten bets from
    out.open("gamestats.dat", ios::out); //File including rounds, avg profit, etc.

    house = 0; //Initial house money
    player = 1000; //Initial player money
    curTime = time(NULL);
    
    cout << "Welcome to Baccarat!" << endl
         << "The time is " << ctime(&curTime);
    //Size of commented out because pointless to include
    //cout << "Size of float is " << sizeof(float) << endl;
    
    do {
        cards[crdSIZE] = randCrd(crdSIZE); //Randomization with max of 14 and min of 2 using function
        suitVal[crdSIZE] = rndSuit(crdSIZE); //Randomization to generate 1-4 for suit interpretation
        suits[crdSIZE] = suitGen(suitVal, crdSIZE); //Generates corresponding suits as string array

        //Process, map inputs to outputs
        //PLACE BETS
        cout << "Player money: $" << fixed << setprecision(2) << player << endl
             << " House money: $" << house << endl << endl;
        cout << "Please input your bets, in dollars, for the following: " << endl;
        cout << "Banker, Player, Tie, Player Pair, and Banker Pair, respectively..." << endl;
        /*
        Commented out if using file as input...
        cin >> betBank >> betPlay >> betTie >> pPair >> bPair;
        */
        
        //Takes bets from file "inputBets.dat"
        inBets >> betBank >> betPlay >> betTie >> pPair >> bPair;
        
        cout << "You placed the following bets:" << endl
             << "     Banker: $" << fixed << setprecision(2) << betBank << " at 1:1 odds" << endl
             << "     Player: $" << betPlay << " at 1:1 odds" << endl
             << "        Tie: $" << betTie << " at 8:1 odds" << endl
             << "Player Pair: $" << pPair << " at 11:1 odds" << endl
             << "Banker Pair: $" << bPair << " at 11:1 odds" << endl << endl;


        //Display your initial conditions as well as outputs.
        //DRAW CARDS FOR PLAYER
        //DRAW TWO CARDS FOR BANK AND PLAYER: 10's and face cards counted as 0
        //FOR TWO DIGIT NUMBERS, DROP OFF FIRST NUMBER
        view[crdSIZE] = faceGen(cards, crdSIZE);
        //Output values of cards and suits based on Card #
        if (cards[0] < 11) {
            cout << "Player Cards:\n" << "Card 1: " << cards[0] << " of " << suits[0] << endl;
        }
        else {
            cout << "Player Cards:\n" << "Card 1: " << view[0] << " of " << suits[0] << endl;
        }
        if (cards[1] < 11) {
            cout << "Card 2: " << cards[1] << " of " << suits[1] << endl;
        }
        else {
            cout << "Card 2: " << view[1] << " of " << suits[1] << endl;
        }
       
        //Point value calculation using ternary operators in function
        cards[0] >= 10 ? subTot[0] = 0: subTot[0] = cards[0];//Calculates subtotal for card 1
        cards[1] >= 10 ? subTot[1] = 0: subTot[1] = cards[1];//Calculates subtotal for card 2
        cards[4] >= 10 ? subTot[4] = 0: subTot[4] = cards[4];//Calculates subtotal for card 5
        
        //Determine whether to draw 3rd card
        if (subTot[0] + subTot[1] <= 5 || subTot[0] + subTot[1] > 9) {
            if (cards[4] < 11) {
                cout << "Card 3: " << cards[4] << " of " << suits[4] << endl << endl;
            }   
            else {
                cout << "Card 3: " << view[4] << " of " << suits[4] << endl << endl;
            }
        }
        else {
            subTot[4] = 0;
            cout << endl;
        }
        
        //Calculate player points
        pTot = subTot[0] + subTot[1] + subTot[4];
        (pTot > 9) ? (pTot = pTot - 10) : (pTot = pTot); //Check if pts > 9 and truncate
        cout << "Player Points: " << pTot << endl << endl;
        
        
        if (cards[2] < 11) {
            cout << "Banker Cards:\n" << "Card 1: " << cards[2] << " of " << suits[2] << endl;    
        }
        else {
            cout << "Banker Cards:\n" << "Card 1: " << view[2] << " of " << suits[2] << endl;
        }     
        if (cards[3] < 11) {
            cout << "Card 2: " << cards[3] << " of " << suits[3] << endl;
        }
        else {
            cout << "Card 2: " << view[3] << " of " << suits[3] << endl;
        }
        //Point value calculation using ternary operators
        cards[2] >= 10 ? subTot[2] = 0: subTot[2] = cards[2];//Calculates subtotal for card 3
        cards[3] >= 10 ? subTot[3] = 0: subTot[3] = cards[3];//Calculates subtotal for card 4
        cards[5] >= 10 ? subTot[5] = 0: subTot[5] = cards[5];//Calculates subtotal for card 6  
        
        //Determine whether to draw 3rd card
        if (subTot[2] + subTot[3] <= 5 || subTot[2] + subTot[3] > 9) {
            if (cards[5] < 11) {
                cout << "Card 3: " << cards[5] << " of " << suits[5] << endl << endl;
            }   
            else {
                cout << "Card 3: " << view[5] << " of " << suits[5] << endl << endl;
            }
        }
        else {
            subTot[4] = 0;
            cout << endl;
        }
        
        //Calculate banker points
        bTot = subTot[2] + subTot[3] + subTot[5];
        (bTot > 9) ? (bTot = bTot - 10) : (bTot = bTot); //Check if pts > 9 and truncate
        cout << "Banker Points: " << bTot << endl;
        //Check if pairs:
        //Player:
        (cards[0] == cards[1] || cards[0] == cards[4] || cards[1] == cards[4]) ? pBool = true : pBool = false; 
        //Banker:
        (cards[2] == cards[3] || cards[2] == cards[5] || cards[3] == cards[5]) ? bBool = true : bBool = false;
       
        //Winning Calculations
        if (pTot > bTot) {
            payOut[0] = betPlay; //Pay for player win
            payOut[1] = betBank * -1; //Loss for player win
            payOut[2] = betTie * 0;
            if (pBool == true) {
                payOut[0] += pPair * 11;
            }
            else {
                payOut[0] -= pPair;
            }
            if (bBool == true) {
                payOut[0] += bPair * 11;
            }
            else {
                payOut[0] -= bPair;
            }
            cout << "Player wins!" << endl << "Winnings: $" << payOut[0] + payOut[1] + payOut[2] << endl << endl;
            }
        else if (bTot > pTot) {
            payOut[0] = betPlay * -1;
            payOut[1] = betBank;
            payOut[2] = betTie * 0;
            if (pBool == true) {
                payOut[1] += pPair * 11;
            }
            else {
                payOut[1] -= pPair;
            }
            if (bBool == true) {
                payOut[1] += bPair * 11;
            }
            else {
                payOut[1] -= bPair;
            }
            cout << "Banker wins!" << endl << "Winnings: $" << payOut[0] + payOut[1] + payOut[2]  << endl;
        }
        else if (bTot == pTot) {
            payOut[0] = betPlay * -1;
            payOut[1] = betBank * -1;
            payOut[2] = betTie * 8;
            if (pBool == true) {
                payOut[2] += pPair * 11;
            }
            else {
                payOut[2] -= pPair;
            }
            if (bBool == true) {
                payOut[2] += bPair * 11;
            }
            else {
                payOut[2] -= bPair;
            }
            cout << "It's a tie!" << endl << "Winnings: $" << payOut[0] + payOut[1] + payOut[2]  << endl; 
        }

        //End of round cumulative monetary totals
        house -= payOut[0] + payOut[1] + payOut[2];
        player += payOut[0] + payOut[1] + payOut[2];
        cout << endl << "End of Round Totals:" << endl << "Player: $" << player << endl
             << "House: $" << house << endl;

        cout << "Would you like to play again? (y/n)" << endl;
        cin >> repeat;
        while (repeat != 'y' && repeat != 'n' && repeat != 'Y' && repeat != 'N') {
            cout << "Please enter y/n\n"
                 << "Would you like to play again? (y/n)";
            cin >> repeat;
        }
        rounds++;
    } while (repeat == 'y' || repeat == 'Y'); //Checks condition after running through code
    //Only accepts capital or lowercase y to play again
    
    inBets.close();
    
    cout << endl << "Thank you for playing Baccarat!" << endl
         << endl << "Total Rounds (Denoted by X):" << endl;
    
    for (int i=0; i<rounds; i++) { //Visual representation of rounds played
        cout << "                         X" << endl;
    }
    
    avgWin = (player-1000)/static_cast<float>(rounds); //Conversion to float using type casting
    cout << "Average profit per game (rounded to nearest $): " << round(avgWin) << endl
         << endl << "Check file \"gamestats.dat\" for statistics" << endl;
    
    if (out.is_open()) {
        //Following to be written into file "game_stats.dat":
        out << "End of Game Totals:" << endl << "Player: $" << fixed << setprecision(2) << player << endl
            << "House: $" << house << endl << endl
            << "Total rounds played: " << rounds << endl
            << "Average profit per round: $" << avgWin << endl;
        while(getline(out, statsLn)) {
            cout << statsLn << endl;
        }
    }
    else {
        cout << "File error - not open\n"; //Response if directory problem
    }
    
    out.close();  
   
    //Exit stage right
    return 0;
}

unsigned short randCrd(const int n) {
    int nums[n];
    for (int i=0; i<n; i++) {
        nums[i] = rand() % (14 + 1 - 2) + 2;
    }
    return nums[n];
}

unsigned short rndSuit(const int n) {
    unsigned short nums[n];
    for (int i=0; i<n; i++) {
        nums[i] = rand() % 4 + 1;
    }
    return nums[n];
}

string faceGen(unsigned short cards[], const int n) {
    string view[n];
    for (int i=0; i<n; i++) {
        if (cards[i] >= 11) { //Independent if to display face cards
            switch (cards[i]) { //Conversion from denoted number
                case 11:
                    view[i] = "Jack";
                    break;

                case 12:
                    view[i] = "Queen";
                    break;

                case 13:
                    view[i] = "King";
                    break;

                case 14:
                    view[i] = "Ace";
                    break;

            }
        }
    }
    return view[n];
}

string suitGen(unsigned short suitVal[], const int n) {
    string suits[n];
    for (int i=0; i<n; i++) {
        switch (suitVal[i]) {
            case 1:
                suits[i] = "Hearts";
                break;

            case 2:
                suits[i] = "Clubs";
                break;

            case 3:
                suits[i] = "Diamonds";
                break;

            case 4:
                suits[i] = "Spades";
                break;
        }
    }
    return suits[n];
}
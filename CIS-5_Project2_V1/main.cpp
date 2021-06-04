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
int randCrd(); //Function to randomize card #
unsigned short rndSuit(const int); //Function to randomize suit - # corresponds to suit
string rndSuit2(unsigned short, const int);
string viewCrd(unsigned short, const int);
void dispCrd(unsigned short, string, string);
unsigned short ptCalc(unsigned short, unsigned short, int);
void thrdCrd(unsigned short, unsigned short, string, string, int, int, int);
void pPtCalc(unsigned short);
void disCrd2(unsigned short, string, string);
//Execution Begins Here
int main(int argc, char** argv) {
    //Set the Random number seed
    srand(time(NULL));
    //Declare variables
    const int crdSIZE = 6;
    float house, //House Money (Cumulative)
          player, //Player money (Cumulative)
          betBank, //Bet on banker - 1:1
          betPlay, //Bet on player - 1:1
          betTie, //Bet on tie between banker + player; 8:1 payout
          pPair, //Bet on pair drawn for player; 11:1 payout
          bPair, //Bet on pair drawn for banker; 11:1 payout
          payOut1, payOut2, payOut3, payTot,
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
        cards = randCrd(); //Randomization with max of 14 and min of 2 using function
        suitVal = rndSuit(crdSIZE); //Randomization to generate 1-4 for suit interpretation
        suits = rndSuit2(suitVal, crdSIZE); //Generates corresponding suits as string array

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
        view = viewCrd(cards, crdSIZE);
        dispCrd(cards, suits, view); //Output values of cards and suits based on Card #
       
        //Point value calculation using ternary operators in function
        subTot[0] = ptCalc(cards, subTot, 0); //Calculates subtotal for card 1
        subTot[1] = ptCalc(cards, subTot, 1); //Calculates subtotal for card 2
        subTot[4] = ptCalc(cards, subTot, 4); //Calculates subtotal for card 5
        
        //Determine whether to draw 3rd card
        thrdCrd(subTot, cards, view, suits, 0, 1, 4);
        
        //Calculate player points
        pPtCalc(subTot);
        disCrd2(cards, suits, view);
       
        //Point value calculation using ternary operators
        subTot[2] = ptCalc(cards, subTot, 2);    
        subTot[3] = ptCalc(cards, subTot, 3);
        subTot[5] = ptCalc(cards, subTot, 5);    

        //Determine whether to draw 3rd card
        if (subTot3 + subTot4 <= 5 || subTot3 + subTot4 > 9) {
            if (card6 < 11) {
                cout << "Card 3: " << card6 << " of " << suit6 << endl << endl;
            }
            else {
                cout << "Card 3: " << view6 << " of " << suit6 << endl << endl;
            }
        }
        else {
            subTot6 = 0;
            cout << endl;
        }
        //Calculate banker points
        bTot = subTot3 + subTot4 + subTot6;
        (bTot > 9) ? (bTot = bTot - 10) : (bTot = bTot); //Check if pts > 9 and truncate
        cout << "Banker Points: " << bTot << endl;

        //Check if pairs:
        (card1 == card2 || card1 == card5 || card2 == card5) ? pBool = true : pBool = false; 
        (card3 == card4 || card3 == card6 || card4 == card6) ? bBool = true : bBool = false;

        //Winning Calculations
        if (pTot > bTot) {
            payOut1 = betPlay; //Pay for player win
            payOut2 = betBank * -1; //Loss for player win
            payOut3 = betTie * 0;
            if (pBool == true) {
                payOut1 += pPair * 11;
            }
            else {
                payOut1 -= pPair;
            }
            if (bBool == true) {
                payOut1 += bPair * 11;
            }
            else {
                payOut1 -= bPair;
            }
            cout << "Player wins!" << endl << "Winnings: $" << payOut1 + payOut2 + payOut3 << endl << endl;
            }
        else if (bTot > pTot) {
            payOut1 = betPlay * -1;
            payOut2 = betBank;
            payOut3 = betTie * 0;
            if (pBool == true) {
                payOut2 += pPair * 11;
            }
            else {
                payOut2 -= pPair;
            }
            if (bBool == true) {
                payOut2 += bPair * 11;
            }
            else {
                payOut2 -= bPair;
            }
            cout << "Banker wins!" << endl << "Winnings: $" << payOut1 + payOut2 + payOut3  << endl;
        }
        else if (bTot == pTot) {
            payOut1 = betPlay * -1;
            payOut2 = betBank * -1;
            payOut3 = betTie * 8;
            if (pBool == true) {
                payOut3 += pPair * 11;
            }
            else {
                payOut3 -= pPair;
            }
            if (bBool == true) {
                payOut3 += bPair * 11;
            }
            else {
                payOut3 -= bPair;
            }
            cout << "It's a tie!" << endl << "Winnings: $" << payOut1 + payOut2 + payOut3  << endl; 
        }

        //End of round cumulative monetary totals
        house -= payOut1 + payOut2 + payOut3;
        player += payOut1 + payOut2 + payOut3;
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

int randCrd() {
    int num = rand() % (14 + 1 - 2) + 2;
    return num;
}

unsigned short rndSuit(const int n) {
    unsigned short nums[n];
    for (int i=0; i<n; i++) {
        nums[i] = rand() % 4 + 1;
    }
    return nums;
}

string rndSuit2(unsigned short suitVal[], const int n) {
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
    return suits;
}

string viewCrd(unsigned short cards[], const int n) {
    string display[n];
    for (int i=0; i<n; i++) {
        if (cards[i] >= 11) { //Independent if to display face cards
               switch (cards[i]) { //Conversion from denoted number
                   case 11:
                       display[i] = "Jack";
                       break;

                   case 12:
                       display[i] = "Queen";
                       break;

                   case 13:
                       display[i] = "King";
                       break;

                   case 14:
                       display[i] = "Ace";
                       break;

               }
           } //view 1 unnecessary in else case so independent if
    }
    return display;
}

void dispCrd(unsigned short cards[], string suits[], string view[]) {
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
}

unsigned short ptCalc(unsigned short cards[], unsigned short subTot[], int val) {    
    cards[val] >= 10 ? subTot[val] = 0: subTot[val] = cards[val];
    return subTot[val];
}

void thrdCrd(unsigned short subTot[], unsigned short cards[], string view[], string suits[], int val1, int val2, int val3) {
    if (subTot[val1] + subTot[val2] <= 5 || subTot[val1] + subTot[val2] > 9) {
                if (cards[val3] < 11) {
                    cout << "Card 3: " << cards[val3] << " of " << suits[val3] << endl << endl;
                }
                else {
                    cout << "Card 3: " << view[val3] << " of " << suits[val3] << endl << endl;
                }
            }

            else {
                subTot[val3] = 0;
                cout << endl;
            }
}

void pPtCalc(unsigned short subTot[]) {
    int pTot;
    pTot = subTot[0] + subTot[1] + subTot[4];
            (pTot > 9) ? (pTot = pTot - 10) : (pTot = pTot); //Check if pts > 9 and truncate
            cout << "Player Points: " << pTot << endl << endl;
}

void disCrd2(unsigned short cards[], string suits[], string view[]) {
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
}
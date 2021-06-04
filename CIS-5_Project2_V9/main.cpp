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
#include <stdlib.h>    //srand, rand, exit function
#include <time.h>      //time for random number seed purposes
#include <string>      //Displaying cards drawn, etc.
#include <fstream>     //To read and write statistics files
#include <ctime>       //Print out time
#include <cmath>       //Used for rounding to nearest $
#include <vector>      //STL Dynamic Array
using namespace std;   //Library Name-space

//User Libraries

//Global/Universal Constants -- No Global Variables
//Science, Math, Conversions, Higher Dimensioned constants only

//Function Prototypes
unsigned short rndCrd(vector<unsigned short> &); //Function to randomize card #
unsigned short rndSuit(); //Function to randomize suit - # corresponds to suit
string suitGen(unsigned short [], const int); //Generates suits from corresponding numbers
string faceGen(vector<unsigned short>, int); //Generates face cards as strings from corresponding numbers
bool pairs(unsigned short [],int,int,int); //Determines whether pairs are present and returns true or false
void endGame(unsigned short, float, string); //Outputs average winnings and goodbyes
void bublSrt(int [],int [],string [], const int); //Bubble Sort - repeatedly swaps adjacent elements if misordered
void selSrt(int [],int [],string [],const int); //Selection Sort in ascending order by finding min
bool binSrch(int [], string [], const int, int, int); //Binary search which displays index if found
void viewScr(); //View high scores
float hPay(float [],int,int,int); //Used to calculate new player and house totals
void refFunc(unsigned short &,unsigned short [],int,int,int); //Uses pass by reference to calc bank total points
void prnt(float,float); //Function overloading! Prints player and house money
void prnt(string); //Function overloading! User greeting outputting user input or default
void scrSrch(int [],string [],const int);

//Execution Begins Here
int main(int argc, char** argv) {
    //Set the Random number seed
    srand(time(NULL));
    //Declare variables
    const int crdSIZE = 6;
    const int paySIZE = 3;
    float house, //House Money (Cumulative)
          player, //Player money (Cumulative)
          betBank, //Bet on banker - 1:0.95
          betPlay, //Bet on player - 1:1
          betTie, //Bet on tie between banker + player; 8:1 payout
          pPair, //Bet on pair drawn for player; 11:1 payout
          bPair, //Bet on pair drawn for banker; 11:1 payout
          payOut[paySIZE], payTot,
          avgWin; //average winnings per round
    unsigned short bCards, //Banker cards drawn
                   pCards, //Player cards drawn
                   rounds, //rounds played (incremented)
                   //cards[crdSIZE], //number on cards
                   suitVal[crdSIZE], //array of randomly generated numbers corresponding to suits
                   pTot, //Player total
                   bTot, //Banker total
                   subTot[crdSIZE]; //Used for calculating zero values for face cards 
    bool pBool,  //Player boolean to check if pair
         bBool; //Bank boolean for pair
    string pName, //Player name
           suits[crdSIZE], //array of suits corresponding to drawn cards
           view[crdSIZE], //Face Card display stored in array
           statsLn; //Stats line from file
    char repeat; //y or n entered by user to indicate repeat
    fstream out, //For writing stats in file - output
            inBets; //File for taking in bets
    time_t curTime; //Current time
    int crdSZ = 6; //Size of array
    vector<unsigned short> cards(crdSZ); //Array -number on cards
    
    //Initialize variables
    /*GAME PLAYED WITH 6-8 DECKS OF CARDS TO NO NEED TO MANIPULATE
      PROBABILITIES BASED UPON PAST DRAWS*/
    rounds = 0;

    inBets.open("inputBets.dat", ios::in); //File to take prewritten bets from
    out.open("gamestats.dat", ios::app); //Appended file displaying names and scores
    
    house = 0; //Initial house money
    player = 1000; //Initial player money
    curTime = time(NULL);
    
    cout << "Welcome to Baccarat!" << endl
         << "The time is " << ctime(&curTime)
         << "Please enter name...";
    cin >> pName;
    prnt(pName);

    //Size of commented out because pointless to include
    //cout << "Size of float is " << sizeof(float) << endl;
    
    do {
        rndCrd(cards);
        
        for (int i=0; i<crdSIZE; i++) {
            suitVal[i] = rndSuit(); //Random number generation from 1-4
        } //Preparation to be translated into suit
        
        for (int i=0; i<crdSIZE; i++) {
            suits[i] = suitGen(suitVal, i); //
        }

        //Process, map inputs to outputs
        //PLACE BETS
        prnt(player, house); //Prints out player and house money 
        cout << "Please input your bets, in dollars, for the following: " << endl;
        cout << "Banker, Player, Tie, Player Pair, and Banker Pair, respectively..." << endl;
        /*
        Commented out if using file as input...
        cin >> betBank >> betPlay >> betTie >> pPair >> bPair;
        */
        
        //Takes bets from file "inputBets.dat"
        inBets >> betBank >> betPlay >> betTie >> pPair >> bPair;
        
        cout << "You placed the following bets:" << endl
             << "     Banker: $" << fixed << setprecision(2) << betBank << " at 1:0.95 odds" << endl
             << "     Player: $" << betPlay << " at 1:1 odds" << endl
             << "        Tie: $" << betTie << " at 8:1 odds" << endl
             << "Player Pair: $" << pPair << " at 11:1 odds" << endl
             << "Banker Pair: $" << bPair << " at 11:1 odds" << endl << endl;


        //Display your initial conditions as well as outputs.
        //DRAW CARDS FOR PLAYER
        //DRAW TWO CARDS FOR BANK AND PLAYER: 10's and face cards counted as 0
        //FOR TWO DIGIT NUMBERS, DROP OFF FIRST NUMBER
        for (int i=0; i<crdSIZE; i++) {
            view[i] = faceGen(cards, i); //function to generate string for corresponding face cards
        }
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
        refFunc(pTot, subTot, 0, 1, 4); //Uses pass by reference to perform player total calc
        //pTot = subTot[0] + subTot[1] + subTot[4];
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
        refFunc(bTot, subTot, 2, 3, 5); //Uses pass by reference to perform bank total calc
        //bTot = subTot[2] + subTot[3] + subTot[5];
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
            payOut[1] = betBank * -0.95; //Loss for player win
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
            payOut[1] = betBank * 0.95;
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
            payOut[1] = betBank * -0.95;
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
        house -= hPay(payOut, 0, 1, 2); //Function using pass by value
        player += hPay(payOut, 0, 1, 2);
        
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
    
    inBets.close(); //Close input file
    
    endGame(rounds, player, pName); //Function for endgame calculations and goodbyes
    
    if (out.is_open()) {
        out << pName << " " << player << " " << rounds << endl; //Write name and end winnings into file "gamestats.dat"
    }
    else {
        cout << "Unable to open output file...";
    }
    
    out.close(); //Close output file "gamestats.dat"  
    
    viewScr(); //Display high scores, sort by score, binary search for value
    
    //Exit stage right
    return 0;
}

void viewScr() {
    char view,//Yes/no - view high scores
         asc, //Yes/no to view in ascending order using bubble sort
         scr; //Yes/no for binary score search
    int sScr, //Score to search for
        index; //Value to return for binary search
    fstream hScrs; //File to be manipulated
    int SIZE = 30; //Array size - alterable depending on number of scores to hold
    int SIZE2 = 2; //2d array 2nd dimension size
    string names[SIZE]; //Names stored in array for sorting
    int scores[SIZE]; //Scores stored in array for sorting
    int rounds[SIZE]; //Rounds stored in array for sorting
    int scrRnd[SIZE][SIZE2]; //2d array
    bool fnd; //True or false if score found
    
        cout << "Would you like to view high scores? (y/n)\n";
        //display scores based on user input or repeat prompt 
        cin >> view;
        while (view!= 'y' && view!='n' && view!='Y' && view!='N') {
            cout << "Please enter y/n\n"
                 << "Would you like to view high scores? (y/n)\n";
            cin >> view;
        }
        if (view=='y'|| view=='Y') {
            hScrs.open("gamestats.dat", ios::in); //reopen to read in           
            for (int i=0; i<SIZE; i++) {
                getline(hScrs, names[i]);
                hScrs >> names[i];
                hScrs >> scores[i];
                hScrs >> rounds[i];
            }

            selSrt(scores, rounds, names, SIZE); //Sorts from high to low - converted to sort trio as linked
            
            for (int i=0; i<SIZE; i++) { //2d array initialization for accessibility and convenience
                scrRnd[i][0] = scores[i];
                scrRnd[i][1] = rounds[i];
            }
            cout << "High Scores (Descending):" << endl;
            for (int j=0; j<SIZE; j++) { //Displays names, scores, and rounds, respectively
                cout << setw(10) << names[j] << " " << setw(6) << scrRnd[j][0] << " " << setw(3) << scrRnd[j][1] << endl; //Output using 2d array 
                //cout << setw(15) << names[j] << " " << scores[j] << " " << rounds[j] << endl;     Original output using single dimensional array        
            }
            scrSrch(scores, names, SIZE);   

                cout << "Would you like to view scores by username in alphabetical order? (y/n)" << endl;
                cin >> asc;
                while (asc!= 'y' && asc!='n' && asc!='Y' && asc!='N') {
                    cout << "Please enter y/n\n"
                         << "Would you like to view scores by username in alphabetical order? (y/n)" << endl;
                    cin >> asc;
                }
                if (asc=='y' || asc=='Y') {
                    bublSrt(scores, rounds, names, SIZE); //Bubble sort
                    cout << "High Scores (Alphabetical):" << endl;
                    for (int k=0; k<SIZE; k++) {
                        cout << setw(10) << names[k] << " " << setw(6) << scores[k] << " " << setw(3) << rounds[k] << endl; //Single dimensional array output post-bubble sort
                    }
                    scrSrch(scores, names, SIZE);                      
                }
                else {
                    cout << "You selected \"No\"...\n" << endl;
                    scrSrch(scores, names, SIZE);
                    cout << "from bottom of chart...\n"
                         << "Goodbye" << endl;
                }
        }
        else {
            cout << "You selected \"No\"..." << endl
                 << "Goodbye" << endl;
            exit(0);
        }
}

unsigned short rndCrd(vector<unsigned short> &crd) {
    for (int i=0; i<crd.size(); i++) {
        crd[i] = rand()%(14+1-2)+2; //Randomization function for cards
    } //Outputs card value and inputs into array through iteration
}


unsigned short rndSuit() {
    unsigned short num;
        num = rand() % 4 + 1;
    return num;
}

string faceGen(vector<unsigned short> crds, int x) {
    string face;
        if (crds[x] >= 11) { //Independent if to display face cards
            switch (crds[x]) { //Conversion from denoted number
                case 11:
                    face = "Jack";
                    break;

                case 12:
                    face = "Queen";
                    break;

                case 13:
                    face = "King";
                    break;

                case 14:
                    face = "Ace";
                    break;

            }
        }
    return face;
}

string suitGen(unsigned short stVal[], const int n) {
    string suit;
        switch (stVal[n]) {
            case 1:
                suit = "Hearts";
                break;

            case 2:
                suit = "Clubs";
                break;

            case 3:
                suit = "Diamonds";
                break;

            case 4:
                suit = "Spades";
                break;
        }
    return suit;
}

void endGame(unsigned short rnds, float pWinngs, string name) {
    float avgWin;
    cout << endl << "Thank you for playing Baccarat, " << name << "!" << endl
         << endl << "Total Rounds (Denoted by X):" << endl;
    
    for (int i=0; i<rnds; i++) { //Visual representation of rounds played
        cout << "                         X" << endl;
    }
    
    avgWin = (pWinngs-1000)/static_cast<float>(rnds); //Conversion to float using type casting
    cout << "Average profit per game (rounded to nearest $): " << round(avgWin) << endl
         << endl << "Check file \"gamestats.dat\" for statistics" << endl;
    
}

void bublSrt(int a[], int r[], string nms[], const int n) {
    int temp; //For swapping
    int temp2;
    string temp3;
    for (int i=0; i<n-1; i++) {
        for (int j=0; j<n-i-1; j++) {
            if (nms[j]>nms[j+1]) {
                temp = a[j];
                temp2 = r[j];
                temp3 = nms[j];
                a[j] = a[j+1];
                r[j] = r[j+1];
                nms[j] = nms[j+1];
                a[j+1] = temp;
                r[j+1] = temp2;
                nms[j+1] = temp3;
            }
        }
    }
}

void selSrt(int a[], int b[], string c[], const int n) { //Conversion for dual
    int temp; //For swapping int 1 in process of sorting
    int temp2; //For swapping int 2
    string temp3; //For names swapping string input (corresponding names)
    for (int i=0; i<n-1; i++) {
        for (int j = i+1; j<n; j++) {
            if (a[j] > a[i]) { //Sort from high to low for high scores
            //Swapping
            temp = a[i];
            temp2 = b[i];
            temp3 = c[i];
            a[i] = a[j];
            b[i] = b[j];
            c[i] = c[j];
            a[j] = temp;
            b[j] = temp2;
            c[j] = temp3;
            }
        }
    }
}

bool binSrch(int a[], string b[], int n, int value, int index) {
    bool found = false;
    for (int i=0; i<n; i++) {
        if (a[i]==value) {
            found = true;
            index=i;
            cout << value << " at completion by " << b[i] << " in position " << index << endl;
            exit(0);
        }
        else {
            found = false;
        }
    }
    return found;
}

float hPay(float pay[], int a, int b, int c) { //Function utilizing pass by value to calculate winnings
    int hCalc;
    hCalc = pay[a] + pay[b] + pay[c];
    return hCalc;
}

void refFunc(unsigned short& oT, unsigned short sT[], int a, int b, int c) {
    oT = sT[a] + sT[b] + sT[c]; //Overall total calc from subtotal input
} //Function utilizing pass by reference to make changes

void prnt(string usrName = "Player 1") { //Function overloading - output with string input
    cout << "Get ready, " << usrName << "...\n\n"; //Set with default argument
}

void prnt(float pMon, float hMon) { //Function overloading - output with unsigned short input
    cout << "Player money: $" << fixed << setprecision(2) << pMon << endl
         << " House money: $" << hMon << endl << endl;
}

void scrSrch(int scrA [], string nms[], const int n) {
    int index;
    char scr; //Variable to hold yes/no
    int sScr; //User entry for score search
    bool fnd; //True or false if score found
    cout << "Would you like to search for a score? (y/n)\n"; 
    cin >> scr; //Enter y or Y to run binary search function
    while (scr!= 'y' && scr!='n' && scr!='Y' && scr!='N') {
        cout << "Please enter y/n\n"
             << "Would you like to search for a score? (y/n)\n"; 
    cin >> scr;
    }
    if (scr=='y' || scr=='Y') {
        cout << "Enter a score to search for...\n";
        cin >> sScr;

        fnd = binSrch(scrA, nms, n, sScr, index); //runs function to find presence and index of score entered                        
        if (fnd==false) {
            cout << "Score not found...\nGoodbye" << endl;
        }
    }
    else {
        cout << "You entered \"No\"...\n"
             << "Goodbye\n";
;    }
}


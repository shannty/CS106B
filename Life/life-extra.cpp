// Shan Rajapakshe
// Game of Life: v3
// Whats new in v3: Added compatability with lifegui.cpp and added random board generation.
// Code for GUI implementation found on lines 47, 91, 117-122
// and function calls for readInputFile, displayGrid, updateGrid, and promptUser.
// Code for Random board generation can be found on lines 81-89, 214-232
// Also provided inline comments to flag GUI and Random code for easier reading.
// June 26th-30th

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "random.h"
using namespace std;

static const char LIFE = 'X';
static const char NOLIFE = '-';

/*
 * Function Declarations
 */
static void openingMessage();
static void readInputFile(Grid<char>& board, LifeGUI& gooeyDisplay);        // GUI
static void copytoBoard(Grid<char>& board, string& line, ifstream &input, int rows, int columns);
static void displayGrid(const Grid<char>& board, LifeGUI& gooeyDisplay);    // GUI
static void updateGrid(Grid<char>& board, LifeGUI& gooeyDisplay);           // GUI
static void countNeighbors(const Grid<char>& board, int i, int j, int& neighbors);
static void promptUser(Grid<char>& board, LifeGUI& gooeyDisplay);           // GUI
static void generateWorld(Grid<char>& board, LifeGUI& gooeyDisplay);        // RANDOM


/*
 * Main function: Sets up size of console window, and creates an empty grid.
 * Then, calls other functions to do various tasks in running the program.
 * Ends with the farewell message "Have a nice Life!"
 */
int main() {
    setConsoleSize(700, 400);

    Grid <char> board;
    LifeGUI gooeyDisplay;       // GUI

    openingMessage();
    readInputFile(board,gooeyDisplay);
    displayGrid(board,gooeyDisplay);
    promptUser(board,gooeyDisplay);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * static void openingMessage(): Prints the opening message of the game,
 * including the rules of the Game of Life
 */
static void openingMessage() {
    cout << "Welcome to the CS 106B Game of Life,\n"
            "a simulation of the lifecycle of a bacteria colony.\n"
            "Cells (X) live and die by the following rules:\n"
            "- A cell with 1 or fewer neighbors dies.\n"
            "- Locations with 2 neighbors remain stable.\n"
            "- Locations with 3 neighbors will create life.\n"
            "- A cell with 4 or more neighbors dies.\n\n";
}

/*
 * static void readInputFile(Grid<char>& board): Takes in a Grid reference as a parameter.
 * Reads the user's desired file. Creates, then populates a grid using the dimensions and
 * state given by the file the user decided.
 */
static void readInputFile(Grid<char>& board, LifeGUI& gooeyDisplay){
    ifstream input;
    string userChoice = getLine("Grid input file name? ");      // RANDOM -->
    while (!openFile(input,userChoice) && (userChoice != "random")) {
        cout << "Unable to open that file.  Try again." << endl;
        userChoice = getLine("Grid input file name? ");
    }
    if (userChoice == "random") {
        cout << "Hello!" <<endl;
        generateWorld(board,gooeyDisplay);
    }                                                           // <-- RANDOM
    else {
        string line;

        getline(input, line);
        int rows = stringToInteger(line);

        getline(input,line);
        int columns = stringToInteger(line);
        //
        board.resize(rows, columns);
        copytoBoard(board, line, input, rows, columns);

        gooeyDisplay.resize(rows,columns);                      // GUI
    }
}

/*
 * static void copytoBoard(Grid<char>& board, string& line, ifstream& input, int rows, int columns):
 * Helper function for readInputFile(), copies initial state of the game to the Grid board.
 */
static void copytoBoard(Grid<char>& board, string& line, ifstream& input, int rows, int columns) {
    for (int i = 0; i < rows; i++) {        // for loop used to iterate over input file and store values
        getline(input, line);               // in board. Starts from the top left, moves left to right
        for (int j = 0; j < columns; j++) { // from top row to bottom row
            board[i][j] = line[j];
        }
    }
}

/*
 * static void displayGrid(const Grid<char>& board): Takes in a grid reference as a parameter. Iterates through
 * Grid from left to right and top to bottom, printing every element.
 */
static void displayGrid(const Grid<char>& board, LifeGUI& gooeyDisplay){

    for (int i = 0; i < board.numRows(); i++) {     // for loop to iterate over the elements of board
        for (int j = 0; j < board.numCols(); j++) { // starts from the top left, moves left to right
            cout << board.get(i,j);                 // from the top row to the bottom
            if (board.get(i,j) == LIFE) {                   // GUI -->
                gooeyDisplay.drawCell(i,j,true);
            }
            else {
                gooeyDisplay.drawCell(i,j,false);           // <--GUI
            }

        }
        cout << endl;
    }
}

/*
 * static void updateGrid(Grid<char>& board): Takes a Grid reference as a paramter. Creates a temporary
 * Grid called temp that is updated based on the contents of original Grid, board.
 * This temporary grid is then copied back into the original grid, so the changes persist.
 * Makes calls to functions countNeighbors(); to count the neighbors around each element
 * and displayGrid to display the contents of the updated grid.
 */
static void updateGrid(Grid<char>& board, LifeGUI& gooeyDisplay) {
    Grid<char> temp = board;
    int neighbors;
    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            countNeighbors(board, i, j, neighbors);         // Call to countNeighbor() helper function
            if (neighbors == 0 || neighbors == 1) {         // Logic to determine state of element in
                temp[i][j] = NOLIFE;                        // next generation
            }
            else if (neighbors == 3) {
                temp[i][j] = LIFE;
            }
            else if (neighbors >= 4) {
                temp[i][j] = NOLIFE;
            }
        }
    }
    board = temp;
    displayGrid(board,gooeyDisplay);
}

/*
 * static void countNeighbors(const Grid<char>& board, const int& i, const int& j, int& neighbors):
 * Helper function for updateGrid(); that counts the neighors of a particular spot in the grid.
 * Does so by looking at the contents of the surrounding 8 elements (if they are in bounds)
 */
static void countNeighbors(const Grid<char>& board, int i, int j, int& neighbors) {
    neighbors = 0;
    for (int drow = i-1; drow <= i+1; drow++) {             //
        for (int dcol = j-1; dcol <= j+1; dcol++) {
            if ((board.inBounds(drow, dcol) && ((drow != i) || (dcol != j)))) { // if the element being looked
                if(board.get(drow,dcol) == LIFE) {                          // at is element (i,j), it is skipped
                    ++neighbors;                                            // so it does NOT count itself
                }
            }
        }
    }
}

/*
 * static void promptUser(Grid<char>& board): Function that displays opening menu for user, and
 * repeatedly prompts user for valid input (a,t or q)
 */
static void promptUser(Grid<char>& board, LifeGUI& gooeyDisplay) {
    string userDecision = getLine("a)nimate, t)ick, q)uit? ");
    while (userDecision != "q") {
        if (userDecision == "t") {
            updateGrid(board,gooeyDisplay);
        }
        else if (userDecision == "a") {
            int animateDecision = getInteger("How many frames? ");
            for (int i = 0; i < animateDecision; i++) {     // Logic to determine how long to animate for
                clearConsole();
                updateGrid(board,gooeyDisplay);
                pause(50);
            }
        }
        else {
            cout << "Invalid choice; please try again." << endl;
        }
        userDecision = getLine("a)nimate, t)ick, q)uit? ");
    }
}

/*
 * static void generateWorld(Grid<char>& board, LifeGUI& gooeyDisplay): Function in charge of generating
 * a random game world. Chooses two random integers between 5 and 20 to be the random world's rows and columns.
 * Then populates world with either '-'s or 'X's with a slight preference for the '-'s
 */
static void generateWorld(Grid<char>& board, LifeGUI& gooeyDisplay) {
    int ranRows = randomInteger(5,20);
    int ranCols = randomInteger(5,20);
    board.resize(ranRows, ranCols);

    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            int ranChoice = randomInteger(0,2);
            if (ranChoice == 2) {
                board[i][j] = LIFE;
            }
            else {
                board[i][j] = NOLIFE;
            }
        }
    }
    gooeyDisplay.resize(ranRows,ranCols);
    displayGrid(board,gooeyDisplay);
}

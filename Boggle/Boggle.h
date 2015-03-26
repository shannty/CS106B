// Shan Rajapakshe
// Boggle.h
// Header file for the Boggle object
// Contains all function definistion, public and private memebers.

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <array>
#include "shuffle.h"
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
#include "set.h"
#include "bogglegui.h"

using namespace std;

class Boggle {
public:
    /*
     * Boggle(const Lexicon &dictionary, string boardText): Constructor for the Boggle object. Takes in a lexicon
     * reference and text that will be placed in the board.
     */
    Boggle(const Lexicon &dictionary, string boardText);

    /*
     * char getLetter(int row, int col) const: Accessor function that obtains the letter found at board location
     * row, col.
     */
    char getLetter(int row, int col) const;

    /*
     * bool checkWord(string word): Function that checks if the word passed in is a valid word. Checks dictionary,
     * as well as the set of words the human found and the set of words the computer found.
     */
    bool checkWord(string word);

    /*
     * bool humanWordSearch(string word): Wrapper function that client accesses to perform a recursive search for the human
     * player's word. Calls the private humanWordSearch() function
     */
    bool humanWordSearch(string word);

    /*
     * int humanScore() const: Accessor function to get the total human score
     */
    int humanScore() const;

    /*
     * Set<string> computerWordSearch(): Wrapper function that client accesses to perform a recursive search for every
     * possible valid word on the board. Calls private computerWordSearch() function
     */
    Set<string> computerWordSearch();

    /*
     * int getScoreComputer() const: Accessor function that gets the score obtained by the computer
     */
    int getScoreComputer() const;

    /*
     * int getRows() const: Accessor function that gets the number of rows of the boggle board.
     */
    int getRows() const;

    /*
     * int getCols() const: Accessor function that gets the number of columns of the boggle board.
     */
    int getCols() const;

    /*
     * int humanWordsNum() const: Accessor function that gets the number of words human player found.
     */
    int humanWordsNum() const;

    /*
     * int computerWordsNum() const: Accessor function that gets the number of words computer player found.
     */
    int computerWordsNum() const;

    /*
     * Set<string> printHumanWords() const: Accessor function that gets the set of human words found.
     */
    Set<string> printHumanWords() const;

    /*
     * Set<string> printComputerWords() const: Accessor function that gets the set of computer words found.
     */
    Set<string> printComputerWords() const;

    /*
     * string getLastWord() const: Accessor that returns the last word found by human player.
     */
    string getLastWord() const;


    // TODO: add any other member functions/variables necessary

private:

    /*
     * void randomBoard(): Function that randomly creates a string using the provided boggle cubes to fill the board
     * with text.
     */
    void randomBoard();

    /*
     * bool humanWordSearch(string word, int index, int i, int j, HashSet<int> &triedBefore): Recursive helper function
     * for humanWordSearch().
     */
    bool humanWordSearch(string word, int index, int i, int j, HashSet<int> &triedBefore);

    /*
     * void computerWordSearch(string currWord, HashSet<int> &checked, int i, int j): Recursive helper function for
     * computerWordSearch().
     */
    void computerWordSearch(string currWord, HashSet<int> &checked, int i, int j);
    const string EMPTY = "";

    /*
     * const int MINSIZE: Smallest size of a valid boggle word
     */
    const int MINSIZE = 4;

    /*
     * const int BOARDSIZE: Number of spaces in a 4 X 4 boggle board
     */
    const int BOARDSIZE = 16;

    /*
     * Grid<char> board: The space where the characters on the board are stored
     */
    Grid<char> board;

    /*
     * Lexicon dictionary: The provided dictionary of boggle words
     */
    Lexicon dictionary;

    /*
     * Set<string> humanWords: The set of words the human player has found
     */
    Set<string> humanWords;

    /*
     * Set<string> computerWords: The set of words the computer player has found
     */
    Set<string> computerWords;

    /*
     *  int huScore: The human player's score
     */
    int huScore = 0;

    /*
     *  int compScore: The computer player's score
     */
    int compScore = 0;

    /*
     * string lastWord: Last word human player found
     */
    string lastWord;
};

/*
 * ostream& operator<<(ostream& out, Boggle& boggle): Operator output function
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h

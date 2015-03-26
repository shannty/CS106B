// Shan Rajapakshe
// ngrams: a Program that takes in a file as input, then outputs text that simulates the author's writing style!
// Sources: Lecture 5, Lecture 6, Lecture 7

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "vector.h"
#include "hashmap.h"
#include "random.h"
using namespace std;

static void promptForInput(ifstream &input, int &n, string &filename);
static void readFile(HashMap<Vector<string>, Vector<string> > &nGrams, ifstream &input, int n, string filename);
static void promptForNumber(HashMap<Vector<string>, Vector<string> > &nGrams, int n);
static void generateWords(HashMap<Vector<string>, Vector<string> > &nGrams, int n, int userNumber);

const int FIRST_IN_VECTOR = 0;

int main() {
    ifstream input;
    string filename;
    int n = 0;
    HashMap<Vector<string>, Vector<string> > nGrams;

    cout << "Welcome to CS 106B Random Writer ('N-Grams')."  << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you."<< endl;

    promptForInput(input, n, filename);
    readFile(nGrams,input, n, filename);
    promptForNumber(nGrams, n);


    cout << "Exiting." << endl;
    return 0;
}

/*
 * static void promptForInput(ifstream &input, int &n, string &filename): Function that prompts user for the file
 * to be read and the length of the n-grams. Deals with input errors, including integers less than 2 and non-integers.
 */
static void promptForInput(ifstream &input, int &n, string &filename) {
    cout << "Input file name? ";
    filename = promptUserForFile(input);

    while (n < 2) {
        n = getInteger("Value of N? ");
        if (n < 2) {
            cout << "N must be 2 or greater." << endl;
        }
    }
    cout << endl;
}

/*
 * static void readFile(HashMap<Vector<string>, Vector<string> > &nGrams, ifstream &input, int n, string filename):
 * Function that reads the file given by the user. Also wraps around the end of the file to map the ending words with
 * words from the front of the text file as well.
 */
static void readFile(HashMap<Vector<string>, Vector<string> > &nGrams, ifstream &input, int n, string filename) {
    Vector<string> window;
    string temp;

    for (int i = 0; i < n -1 ; i++) {
        input >> temp;
        window.add(temp);
    }
    input >> temp;
    while (true) {
        if (input.fail()) {
            input.close();
            input.open(filename);
            for (int i = 0; i < n - 1; i++) {
                input >> temp;
                nGrams.put(window, nGrams.get(window)+= temp);
                window.remove(FIRST_IN_VECTOR);
                window.add(temp);
            }
            input.close();
            break;
        }
        nGrams.put(window, nGrams.get(window)+= temp);      // If key has been seen before, this ensures that the new
        window.remove(FIRST_IN_VECTOR);                     // value is appended to original value vector, instead of
        window.add(temp);                                   // overwriting it.
        input >> temp;
    }
}

/*
 * static void promptForNumber(HashMap<Vector<string>, Vector<string> > &nGrams, int n):
 * Function that repeatedly prompts user for the number of words to be printed using the map of nGrams.
 * Deals with integers less than n and non-integer errors.
 * Makes a call to generateWords().
 */
static void promptForNumber(HashMap<Vector<string>, Vector<string> > &nGrams, int n) {
    int userNumber = -1;
    while (userNumber < n) {                     // Will repeatedly ask for # of random words
        userNumber = getInteger("# of random words to generate (0 to quit)? ");
        if (userNumber == 0) {
            goto end;                           // if user wants to quit, goes to end, which will terminate program
        }
        else if (userNumber < n) {
            cout << "Must be at least " << n << " words." << endl;
        }
        else {
            generateWords(nGrams, n, userNumber);
            cout << endl;
        }
        userNumber = -1;
    }
    end:;
}

/*
 * static void generateWords(HashMap<Vector<string>, Vector<string> > &nGrams, int n, int userNumber): Function that
 * prints out the words
 */
static void generateWords(HashMap<Vector<string>, Vector<string> > &nGrams, int n, int userNumber) {
    Vector<Vector<string> > keys = nGrams.keys();
    Vector<string> writeWindow;

    writeWindow = keys.get(randomInteger(0,keys.size()));
    cout << "...";

    for (int i = 0; i < n - 1; i++) {       // runs through the writeWindow, and prints the contents
        cout << writeWindow[i] << " ";
    }

    for (int j = 0; j < userNumber - 1; j++) {
        int rand = randomInteger(0,nGrams.get(writeWindow).size() - 1); //chooses a random element in the values vector
        string nextWord = nGrams.get(writeWindow)[rand];                // of the key writeWindow. This element is then
        writeWindow.remove(FIRST_IN_VECTOR);                            // printed out, then added to writeWindow (after
        writeWindow.add(nextWord);                                      // writeWindow removes it's first element.
        cout << nextWord << " ";
    }
    cout << "..." << endl;
}

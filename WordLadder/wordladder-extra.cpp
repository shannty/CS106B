// Shan Rajapakshe
// wordladder.cpp - A program that finds the shortest word ladder between two different words
// of the same size using a breadth-first search approach.
// Each step in the ladder must be a valid english word as specified by the dictionary.txt file
// Sources: Lecture 5, Lecture 6, Lecture 7

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "hashset.h"
using namespace std;

static void storeDictionary(HashSet<string> &dictionary);
static void readUserInput(const HashSet<string> &dictionary);
static void findWordLadder(const HashSet<string> &dictionary, string word1, string word2);
static void printWordLadder(Stack<string> temp, string word1, string word2);

int main() {

    HashSet<string> dictionary;

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the " << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    storeDictionary(dictionary);
    readUserInput(dictionary);      // could either declare the dictionary in main, and
    // call functions in main, or declare dictionary in
    // storeDictionary and call functions from there as well
    // for now, I will be calling from main.

    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * static void storeDictionary(): Function that reads in the provided dictionary.txt file and
 * stores all of the words in a HashSet of strings.
 */
static void storeDictionary(HashSet<string> &dictionary) {

    ifstream input;
    string entry;

    input.open("dictionary.txt");

    while (getline(input,entry)) {
        dictionary.add(entry);
    }
    input.close();
}

/*
 * static void readUserInput(const HashSet<string> &dictionary): Function that repeatedly
 * prompts user to input words. Takes care of input errors by printing an error message and
 * reprompting the user. Calls function findWordLadder() if no errors are found in user input.
 */
static void readUserInput(const HashSet<string> &dictionary) {
    while (true) {
        string word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));     // find a better way to do this!
        if (word1 == "") {
            break;
        }
        string word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (word2 == "") {
            break;
        }

        if (dictionary.contains(word1) && dictionary.contains(word2)) {
            if (word1 == word2) {
                cout << "The two words must be different.\n" << endl;
            }
            else if (word1.length() != word2.length()) {
                cout << "The two words must be the same length.\n" << endl;
            }
            else {
                findWordLadder(dictionary, word1, word2);
            }
        }
        else {
            cout << "The two words must be found in the dictionary.\n" << endl;
        }
    }
}

/*
 * static void findWordLadder(const HashSet<string> &dictionary, string word1, string word2):
 * Function that finds a word ladder from word1 to word2
 */
static void findWordLadder(const HashSet<string> &dictionary, string word1, string word2) {
    Queue<Stack<string> > partialLadders;
    HashSet<string> usedWords;

    Stack<string> startWord;
    startWord.push(word1);

    partialLadders.enqueue(startWord);

    while (!partialLadders.isEmpty()) {
        Stack<string> temp = partialLadders.dequeue();

        string curWord = temp.peek();
        for (int i = 0; i < (temp.peek()).length(); i++) {          // The following checks the
            for (int j = 0; j < 26; j++ ) {                         // 'neighbors' of the current word
                curWord[i] = 97 + j ;
                if (dictionary.contains(curWord)) {
                    if (!usedWords.contains(curWord)) {
                        usedWords.insert(curWord);
                        if (curWord == word2) {
                            printWordLadder(temp, word1, word2);
                            goto end;
                        }
                        else {
                            Stack<string> nextLadder = temp;
                            nextLadder.push(curWord);
                            partialLadders.enqueue(nextLadder);
                        }
                    }
                }
            }
            curWord = temp.peek();
        }
    }
    if (partialLadders.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
    }

    end:
    cout << endl;
}

/*
 * static void printWordLadder(Stack<string> temp, string word1, string word2): Function that
 * prints the word ladder, starting from word2 and working back to word1.
 */
static void printWordLadder(Stack<string> temp, string word1, string word2) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;

    cout << word2 << " ";
    while (!temp.isEmpty()) {
        cout << temp.pop() << " ";
    }
    cout << endl;
}

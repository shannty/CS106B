// Shan Rajapakshe
// boggleplay.cpp
// The client file that uses the Boggle object referred to in Boggle.cpp and Boggle.h

#include <cctype>
#include "lexicon.h"
#include "simpio.h"
#include "console.h"
#include "Boggle.h"
#include "simpio.h"

const string EMPTY = "";
const int ROWCOL = 4;
const int BOARDSIZE = 16;

void playOneGame(Lexicon& dictionary) {

    BoggleGUI::initialize(ROWCOL, ROWCOL);
    cout << "Do you want to generate a random board? ";
    string boardText;
    if (getYesOrNo()) {
        boardText = EMPTY;
    }
    else {
        while (true) {      // loop to ensure proper input
            bool goodString = true;
            boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
            for (int i = 0; i < boardText.length(); i++) {  // loop to check
                if (!isalpha(boardText[i])) {
                    goodString = false;
                }
            }
            if (boardText.length() == BOARDSIZE && goodString ) {  // if length is correct, and it text is a 'good' stringCONSTTTT
                break;
            }
            else {
                cout << "That is not a valid 16-letter board string. Try again." << endl;
            }
        }
    }
    clearConsole();
    Boggle boggle(dictionary, boardText);

    BoggleGUI::setAnimationDelay(200);
    while (true) {
        cout << boggle;
        cout << "It's your turn!" << endl;
        cout << "Your words (" << boggle.humanWordsNum() << "): " << boggle.printHumanWords() << endl;
        cout << "Your score: " << boggle.humanScore() << endl;
        string userDecision = toUpperCase(getLine("Type a word (or Enter to stop): "));

        if (userDecision == EMPTY) {
            break;
        }
        clearConsole();         // clears console
        if (!boggle.humanWordSearch(userDecision)) {
            if (dictionary.contains(userDecision)) {
                cout <<"That word can't be formed on this board." << endl;
                BoggleGUI::clearHighlighting();
            }
            else {
                cout <<"You must enter an unfound 4+ letter word from the dictionary." << endl;
            }
        }
        else {
            cout << "You found a new word! \"" << boggle.getLastWord() << "\" " << endl;
        }
    }
    cout << endl;
    BoggleGUI::clearHighlighting();
    cout << "It's my turn!" << endl;
    boggle.computerWordSearch();

    cout << "My words (" << boggle.computerWordsNum() <<"): " << boggle.printComputerWords() << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;

    if (boggle.getScoreComputer() > boggle.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");

    }
    else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}

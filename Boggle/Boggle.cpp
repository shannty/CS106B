// Shan Rajapakshe
// Boggle.cpp
// Implementation of the Boggle object

#include "Boggle.h"


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


Boggle::Boggle(const Lexicon& dictionary, string boardText) {
    board.resize(4,4);
    this->dictionary = dictionary;

    if (boardText == EMPTY) {
        randomBoard();
    }
    else {
        int count = 0;
        for (int i = 0; i < board.nRows; i++) {
            for (int j = 0; j < board.nCols; j++) {
                board[i][j] = boardText[count];
                count++;
            }
        }
        BoggleGUI::labelAllCubes(boardText);
    }
}

char Boggle::getLetter(int row, int col) const {
    if (row > board.nRows) {
        throw row;
    }
    else if (col > board.nCols) {
        throw col;
    }
    return board[row][col];
}

bool Boggle::checkWord(string word) {
    if (word.length() >= MINSIZE) {
        if (dictionary.contains(word)) {
            if (!humanWords.contains(word) && !computerWords.contains(word)) {
                return true;
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearch(string word) {
    if (checkWord(word)) {
        BoggleGUI::clearHighlighting();
        int index = 0;
        HashSet<int> triedBefore;
        for (int i = 0; i < board.nRows; i++) {
            for (int j = 0; j < board.nCols; j++) {
                BoggleGUI::setHighlighted(i,j,true);
                if (board.get(i,j) == word[index]) {
                    int tried = (i*10) + j;         // stores grid locations tried before as 2 digit integers
                    triedBefore.add(tried);         // adds a the current grid location to set of tried ones for this specific run
                    index++;
                    if (humanWordSearch(word, index, i, j, triedBefore)) {
                        return true;
                    }
                    triedBefore.remove(tried);      // unchoose location from set of tried locations
                    index--;
                }
                BoggleGUI::setHighlighted(i,j,false);
            }
        }
        return false;
    }
    else {
        return false;
    }
}

bool Boggle::humanWordSearch(string word, int index, int i , int j, HashSet<int> &triedBefore) {
    if (index == word.length()) {
        int scoreAdded = word.length() - (MINSIZE - 1);       // determines score to be added. MINSIZE - 1
        huScore += scoreAdded;                              // ensures each letter that is greater than or
        humanWords += word;                                 // equal to 4 gets a point
        lastWord = word;
        BoggleGUI::setScore(huScore, BoggleGUI::HUMAN);
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        return true;
    }
    for (int drow = i-1; drow <= i+1; drow++) {
        for (int dcol = j-1; dcol <= j+1; dcol++) {
            if ((board.inBounds(drow, dcol) && ((drow != i) || (dcol != j)))) {     // checks if new location is inbounds and that drow and dcol do not refer to previous board location
                if (board.get(drow,dcol) == word[index]) {
                    int gridValue = (10*drow) + dcol;
                    if (!triedBefore.contains(gridValue)) {
                        triedBefore.add(gridValue);
                        BoggleGUI::setHighlighted(drow, dcol, true);
                        index++;
                        if (humanWordSearch(word, index, drow, dcol, triedBefore)) {   // explore choices, change index so for loop looks for that char
                            return true;
                        }
                        triedBefore.remove(gridValue);
                        index--;
                        BoggleGUI::setHighlighted(drow, dcol, false);
                    }
                }

            }
        }
    }
    return false;
}

int Boggle::humanScore() const {
    return huScore;
}

Set<string> Boggle::computerWordSearch() {
    HashSet<int> checked;
    string currWord;
    for (int i = 0; i < board.nRows; i++) {
        for (int j = 0; j < board.nCols; j++) {
            int checking = (i*10) + j;
            checked.add(checking);              // adding current grid spot to checked
            currWord += board.get(i,j);         // adding current char to currWord (choosing next char)
            computerWordSearch(currWord, checked, i, j);
            checked.remove(checking);                              // removing current grid spot from checked
            currWord = currWord.substr(0, currWord.length() - 1);  // unchoose char
        }
    }
    BoggleGUI::setScore(compScore, BoggleGUI::COMPUTER);
    return computerWords;
}

void Boggle::computerWordSearch(string currWord, HashSet<int> &checked, int i, int j) {
    if (checkWord(currWord)) {
        int scoreAdded = currWord.length() - (MINSIZE - 1);       // determines score to be added. MINSIZE - 1
        compScore += scoreAdded;                              // ensures each letter that is greater than or
        computerWords += currWord;
        BoggleGUI::recordWord(currWord, BoggleGUI::COMPUTER);
    }
    for (int drow = i-1; drow <= i+1; drow++) {
        for (int dcol = j-1; dcol <= j+1; dcol++) {
            if ((board.inBounds(drow, dcol) && ((drow != i) || (dcol != j)))) {
                int checking = (drow*10) + dcol;
                if (!checked.contains(checking)) {
                    checked.add(checking);              // adding current grid spot to checked
                    currWord += board.get(drow,dcol);         // adding current char to currWord (choosing next char)
                    if (dictionary.containsPrefix(currWord)) {
                        computerWordSearch(currWord, checked, drow, dcol);
                    }
                    checked.remove(checking);                              // removing current grid spot from checked
                    currWord = currWord.substr(0, currWord.length() - 1);  // unchoose char
                }
            }
        }
    }
}

int Boggle::getScoreComputer() const {
    return compScore;
}

void Boggle::randomBoard() {
    string output;
    shuffle(CUBES, BOARDSIZE);
    int counter = 0;
    for (int i = 0; i < board.nRows; i++) {
        for (int j = 0; j < board.nCols; j++) {
            CUBES[counter] = shuffle(CUBES[counter]);

            board[i][j] = CUBES[counter][0];
            output += board[i][j];
            counter++;
        }
    }
    BoggleGUI::labelAllCubes(output);
}

int Boggle::getRows() const{
    return board.nRows;
}

int Boggle::getCols() const {
    return board.numCols();
}

int Boggle::humanWordsNum() const {
    return humanWords.size();
}

int Boggle::computerWordsNum() const {
    return computerWords.size();
}

Set<string> Boggle::printHumanWords() const {
    return humanWords;
}
Set<string> Boggle::printComputerWords() const {
    return computerWords;
}

string Boggle::getLastWord() const {
    return lastWord;
}

ostream& operator << (ostream& out, Boggle& boggle) {
    for (int i = 0; i < boggle.getRows(); i++) {
        for (int j = 0; j < boggle.getCols(); j++) {
            out << boggle.getLetter(i, j);
        }
        out << endl;
    }
    return out;
}

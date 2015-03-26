// Shan Rajapakshe
// Recursive Problems

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
using namespace std;

const char CHAR_TO_INT_CONVERTER = '0';
const char NEGATIVE_INTEGER = '-';
const int BASE = 10;      // used in converStringToInteger

const string EMPTY_STRING = "";
const string PARENTHESES = "()";
const string CURLY_BRACKETS = "{}";
const string SQUARE_BRACKETS = "[]";
const int ERASE_LENGTH = 2;

const double SQUAREROOT_THREE_OVER_TWO = (sqrt(3)/2);

const char END_OF_START_SEQUENCE = ':';
const string SPACE = " ";
const string PIPE = "|";
const int SEPARATOR_REMOVER_INDEX = 3;


int convertStringToInteger(string exp, int &count, int stringLength);   // Recursive Function for convertStringToInteger
int convertStringToInteger(string exp);                                 // Wrapper Function for convertStringToInteger

bool removeParentheses(string exp, string paren);       // Mutually recursive with isBalanced
bool isBalanced(string exp);                            // Used in removeParentheses

double weightOnKnees(int row, int col, Vector<Vector<double> >& weights);   // Recursive function to determine weight on knees

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order);    // Recursive function to draw sierpinski triangls

void floodFill(int x, int y, int width, int height, int color);             // Recursive function to flood fill an area

void storeGrammar(istream &input, HashMap<string, Vector<string> > &grammar);   // Helper function to store grammar file as a map of strings to vectors of strings
void expandGrammar(const HashMap<string, Vector<string> > grammar, string symbol, string &output);  // Recursive helper function, recursively expands a grammar into terminals
Vector<string> grammarGenerate(istream& input, string symbol, int times);   // Function that generates phrases of based on a certain grammar


int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*
 * int convertStringToInteger(string exp, int &count, int stringLength):
 * The actual recursive function that returns the integer version
 * of the string passed in. Called by the wrapper function of the same
 * name.
 */
int convertStringToInteger(string exp, int &count, int stringLength) {
    if ( count == stringLength - 1) {
        return exp[count] - CHAR_TO_INT_CONVERTER;
    }
    else {
        int num = exp[count] - CHAR_TO_INT_CONVERTER;
        int finalNumber = num * pow(BASE, stringLength - count - 1);
        count++;
        return finalNumber + convertStringToInteger(exp, count, stringLength);
    }
}
/*
 * int convertStringToInteger(string exp): Wrapper function for convertStringToInteger()
 * used by clients, which makes a call to the recursive version of this function.
 */
int convertStringToInteger(string exp) {
    int count = 0;
    int stringLength = exp.length();

    if (exp[count] == NEGATIVE_INTEGER ) {
        count++;
        return -1 * convertStringToInteger(exp, count, stringLength);
    }
    else {
        return convertStringToInteger(exp, count, stringLength);
    }
}

/*
 * bool removeParentheses(string exp, string paren): Helper function for isBalanced() that
 * recursively calls isBalanced (uses mutual recursion).
 */
bool removeParentheses(string exp, string paren) {
    int index = stringIndexOf(exp, paren);
    exp.erase(index, ERASE_LENGTH);
    return isBalanced(exp);
}

/*
 * bool isBalanced(string exp): Function that checks if a string has balanced brackets.
 * Recusively calls helper function removeParentheses() via mutual recursion.
 */
bool isBalanced(string exp) {
    if (exp == EMPTY_STRING) {
        return true;
    }
    else {
        if (stringContains(exp, PARENTHESES)) {
            return removeParentheses(exp, PARENTHESES);
        }
        else {
            if (stringContains(exp, CURLY_BRACKETS)) {
                return removeParentheses(exp, CURLY_BRACKETS);
            }
            else {
                if (stringContains(exp, SQUARE_BRACKETS)) {
                    return removeParentheses(exp, SQUARE_BRACKETS);
                }
                else {
                    return false;
                }
            }
        }
    }
}

/*
 * double weightOnKnees(int row, int col, Vector<Vector<double> >& weights): Function that
 * calculates the weight resting on a person who is in a human pyramid.
 */
double weightOnKnees(int row, int col, Vector<Vector<double> >& weights) {
    if (row == 0 && col == 0) {
        return weights[0][0];
    }
    else if (col < 0 || col >= weights[row].size())
        return 0.0;
    else {
        return weights[row][col] + weightOnKnees(row - 1, col - 1, weights)/2 + weightOnKnees(row - 1, col, weights)/2;
    }
}

/*
 * void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order): Function that
 * recursively draws Sierpinski Triangles.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order == 0) {
        return;
    }
    else {
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x + (size/2), y + size * SQUAREROOT_THREE_OVER_TWO);
        gw.drawLine(x + size, y, x + (size/2), y + size * SQUAREROOT_THREE_OVER_TWO);

        order--;
        size = size/2;
        drawSierpinskiTriangle(gw, x, y, size, order);
        drawSierpinskiTriangle(gw, x + size, y, size, order);
        drawSierpinskiTriangle(gw, x + (size/2), y + size * SQUAREROOT_THREE_OVER_TWO, size, order);
    }
}

/*
 * void floodFill(int x, int y, int width, int height, int color): Function that fills in a
 * section of a canvas that is all composed of the same color.
 */
void floodFill(int x, int y, int width, int height, int color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    else {
        int orgColor = getPixelColor(x,y);
        setPixelColor(x, y, color);

        if ((x - 1 > 0) && (x - 1 < width)) {
            if (getPixelColor(x - 1, y) == orgColor) {
                floodFill(x - 1, y, width, height, color);
            }
        }
        if (x + 1 > 0 && x + 1 < width) {
            if (getPixelColor(x + 1, y) == orgColor) {
                floodFill(x + 1, y, width, height, color);
            }
        }
        if (y - 1 > 0 && y - 1 < height) {
            if (getPixelColor(x, y - 1) == orgColor) {
                floodFill(x, y - 1, width, height, color);
            }
        }
        if (y + 1 > 0 && y + 1 < width) {
            if (getPixelColor(x, y + 1 ) == orgColor) {
                floodFill(x, y + 1, width, height, color);
            }
        }
    }
}

/*
 * void storeGrammar(istream &input, HashMap<string, Vector<string> > &grammar): Helper
 * function for grammarGenerate() that stores the provided grammar file as a map.
 */
void storeGrammar(istream &input, HashMap<string, Vector<string> > &grammar){
    string line;
    string start;
    Vector<string> v;

    while (getline(input, line)) {

        start = line.substr(0, line.find(END_OF_START_SEQUENCE));
        if (grammar.containsKey(start)) {
            throw "string exception";
        }
        line.erase(0, line.find(END_OF_START_SEQUENCE) + SEPARATOR_REMOVER_INDEX);
        v = stringSplit(line, PIPE);
        grammar.put(start, v);
    }
}

/*
 * void expandGrammar(Vector<string> &v, const HashMap<string, Vector<string> > grammar, string symbol):
 * Helper function for grammarGenerate() that takes care of the recursive calls.
 */
void expandGrammar(const HashMap<string, Vector<string> > grammar, string symbol, string &output) {

    if (!grammar.containsKey(symbol)) {
        output += symbol;
        output += " ";
        return;
    }
    else {
        int rand = randomInteger(0, grammar.get(symbol).size() - 1);
        string newSymbol = grammar.get(symbol)[rand];

        if (stringContains(newSymbol, SPACE)) {
            Vector<string> temp = stringSplit(newSymbol, SPACE);
            for (string s: temp) {
                expandGrammar(grammar, s, output);
            }
        }
        else {
            expandGrammar(grammar, newSymbol, output);
        }
    }
}

/*
 * Vector<string> grammarGenerate(istream& input, string symbol, int times): Function that prompts
 * user for a grammar, and creates language strucutres based on user input using said grammar.
 * Makes a call to helper function storeGrammar() and recursively calls helper
 * function expandGrammar().
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == EMPTY_STRING) {
        throw "string exception";
    }
    HashMap<string, Vector<string> > grammar;
    storeGrammar(input, grammar);
    Vector<string> v;
    string output;

    for (int i = 0; i < times; i++) {
        expandGrammar(grammar, symbol, output);
        v += output;
        output.clear();
    }
    return v;
}

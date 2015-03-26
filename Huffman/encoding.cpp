/* Shan Rajapakshe
 * Huffman Encoding - Reads a file, creates a specific huffman encoding scheme based on the specific
 * word frequencies found in the file, and encodes the file with said encoding scheme.
 */

#include "encoding.h"
#include "pqueue.h"
#include "strlib.h"
#include "filelib.h"
#include <iostream>

const string STR_ZERO = "0";
const string STR_ONE = "1";
const int ZERO = 0;
const int ONE = 1;
const int CONVERTOR = 48;


/*
 * Map<int, int> buildFrequencyTable(istream& input): Builds a frequency table based on the
 * istream input. Table is used to create the encoding tree.
 * Called by client, and by compress().
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(true) {
        int character = input.get();
        if (input.fail()) {
            break;
        }
        freqTable[character]++;
    }
    freqTable.put(PSEUDO_EOF, ONE);
    return freqTable;
}

/*
 * HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable): Builds the binary encoding
 * tree using the Frequency table created by buildFrequencyTable().
 * Called by client, and by compress().
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> pq;
    for (int character: freqTable) {
        HuffmanNode* nextNode = new HuffmanNode(character, freqTable[character]);
        pq.enqueue(nextNode, nextNode->count);
    }
    while (pq.size() != ONE) {
        HuffmanNode* first = new HuffmanNode(pq.peek()->character, pq.peek()->count, pq.peek()->zero, pq.peek()->one);
        pq.dequeue();
        HuffmanNode* second = new HuffmanNode(pq.peek()->character, pq.peek()->count, pq.peek()->zero, pq.peek()->one);
        pq.dequeue();
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, (first->count + second->count), first, second);
        pq.enqueue(parent, parent->count);
    }
    HuffmanNode* root = new HuffmanNode(pq.peek()->character, pq.peek()->count, pq.peek()->zero, pq.peek()->one);
    return root;
}

/*
 * void buildEncodingMap(HuffmanNode *encodingTree, Map<int, string> &encodingMap, string path):
 * Helper function for buildEncodingMap(). Recursively traverses encoding tree, finding the
 * binary representation of each character, and story each in a map.
 */
void buildEncodingMap(HuffmanNode *encodingTree, Map<int, string> &encodingMap, string path) {
    if (encodingTree->character != NOT_A_CHAR) {
        encodingMap.put(encodingTree->character, path);
        return;
    }
    buildEncodingMap(encodingTree->zero, encodingMap, path += STR_ZERO);
    path = path.substr(ZERO, path.length() - ONE);
    buildEncodingMap(encodingTree->one, encodingMap, path += STR_ONE);
    path = path.substr(ZERO, path.length() - ONE);
}

/*
 * Map<int, string> buildEncodingMap(HuffmanNode* encodingTree): Function that builds the
 * encoding map. Makes a call to the recursive helper function buildEncodingMap().
 * Called by client and by compress().
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string path;
    buildEncodingMap(encodingTree, encodingMap, path);
    return encodingMap;
}

/*
 * void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output):
 * Encodes the istream input via huffman encoding.
 * Called by the client and compress().
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (true) {
        int character = input.get();
        if (input.fail()) {
            break;
        }
        string binaryRep = encodingMap[character];
        for (int i = ZERO; i < binaryRep.length(); i++) {
            int theDigit = binaryRep[i] - CONVERTOR;
            output.writeBit(theDigit);
        }
    }
    string end = encodingMap[PSEUDO_EOF];
    for (int i = ZERO; i < end.length(); i++) {
        int theDigit = end[i] - CONVERTOR;
        output.writeBit(theDigit);
    }
}

/*
 * void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, bool& cont):
 * Helper function that recursively traverses the encoding tree as the input is read, and decodes
 * input along the way.
 */
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, bool& cont) {
    if (encodingTree->character != NOT_A_CHAR) {
        if (encodingTree->character == PSEUDO_EOF) {
            cont = false;
            return;
        }
        output.put(encodingTree->character);
        return;
    }
    int nextBit = input.readBit();
    if (nextBit == ZERO) {
        decodeDataHelper(input, encodingTree->zero, output, cont);
    }
    else {
        decodeDataHelper(input, encodingTree->one, output, cont);
    }
}

/*
 * void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output): Decodes the
 * binary file or string passed as input with the encoding tree. Makes a call to decodeDataHelper().
 * Called by client and by decompress().
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* front = encodingTree;
    bool cont = true;
    while(cont) {
        decodeDataHelper(input, front, output, cont);
        if (input.fail()) {
            break;
        }
    }
}

/*
 * void compress(istream& input, obitstream& output): Compresses the file given as input by
 * via a huffman encoding scheme. Encodes the freqTable as a header for the file. Makes calls
 * to buildFrequencyTable(), buildEncodingTree(), buildEncodingMap(), and freeTree().
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* top = buildEncodingTree(freqTable);
    Map<int, string> encodeMap = buildEncodingMap(top);
    output << freqTable;
    rewindStream(input);
    encodeData(input, encodeMap, output);
    freeTree(top);
}


/*
 * void decompress(ibitstream& input, ostream& output): Decompresses a file given as input, using
 * the file's header as the encoding tree. Makes calls to buildEncodingTree(), decodeData(), and
 * freeTree().
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> frequencyTable;
    input >> frequencyTable;
    HuffmanNode* encodeTree = buildEncodingTree(frequencyTable);
    decodeData(input, encodeTree, output);
    freeTree(encodeTree);
}


/*
 * void freeTree(HuffmanNode* node): Recursively free's the data associated with a
 * binary search tree.
 */
void freeTree(HuffmanNode* node) {
    if (node->character != NOT_A_CHAR) {
        delete[] node;
        return;
    }
    freeTree(node->zero);
    freeTree(node->one);
    delete[] node;
}

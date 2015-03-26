// Shan Rajapakshe
// Priority Queue - Heap implementation

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 * See class file (.cpp) for specific pros and cons of this implementation
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    /*
     * void resize(): Helper function, resizes the queue
     */
    void resize();

    /*
     * void bubbleUp(int current): Helper function, does the work of "bubbling up" a specific index
     */
    void bubbleUp(int current);

    PQEntry* heap;
    int theSize;
    int capacity;

    const int FRONT_INDEX = 1;
    const int TWO = 2;
};

#endif

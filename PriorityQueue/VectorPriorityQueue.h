// Shan Rajapakshe
// Priority Queue- Unsorted Vector implementation

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;


// See class file (.cpp) for specific pros and cons of this implementation

class VectorPriorityQueue {
public:

    /*
     * VectorPriorityQueue(): Constructor, creates the priority queue as an unsorted vector
     */
    VectorPriorityQueue();

    /*
     * ~VectorPriorityQueue(): Descrutor, destroys the priority queue and all associated memory
     * is freed
     */
    ~VectorPriorityQueue();

    /*
     * void changePriority(string value, int newPriority): Function to change the priority of an
     * induvidual in the queue.
     */
    void changePriority(string value, int newPriority);

    /*
     * void clear(): Removes all items from the priority queue
     */
    void clear();

    /*
     * string dequeue(): Dequeues the item with the highest priority
     */
    string dequeue();

    /*
     * void enqueue(string value, int priority): Enqueues an item, giving it a specific priority
     */
    void enqueue(string value, int priority);

    /*
     * bool isEmpty(): Checks if the priority queue is empty. Returns true if empty
     */
    bool isEmpty() const;

    /*
     * string peek(): Returns the next item to be dequeued.
     */
    string peek() const;

    /*
     * int peekPriority(): Returns the priority value of the highest priority element in the queue
     */
    int peekPriority() const;

    /*
     * int size(): Returns the size of the priority queue
     */
    int size() const;

    /*
     * ostream& operator <<: Allows the priority queue to be displayed via the output stream
     */
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);



private:

    /*
     * Vector<PQEntry> priorityQueue: The internal vector, used as the priority queue
     */
    Vector<PQEntry> priorityQueue;

    /*
     * int priorityLooper(): Helper function that loops over vector, finding the highest priority
     * item
     */
    int priorityLooper() const;

};

#endif

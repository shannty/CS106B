/* Shan Rajapakshe
Priority Queue implemented as a Binary Heap

The fasted implementation discussed. Very quick to dequeue, enqueue, peek, change priority, etc. but is a bit difficult to work with due to the
somewhat confusing nature of the heap.
*/

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    heap = new PQEntry[10];
    theSize = 0;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] heap;
}

// O(N*logN)
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 1; i <= theSize; i++) {
        if (heap[i].value == value) {
            if (heap[i].priority < newPriority) {
                throw "New priority is not more urgent than previous priority!";
            }
            else {
                heap[i].priority = newPriority;
                bubbleUp(i);
            }
        }
    }
}

// O(1)
void HeapPriorityQueue::clear() {
    PQEntry* newHeap = new PQEntry[capacity];
    delete[] heap;
    heap = newHeap;
    theSize = 0;
}

// O(log N)
string HeapPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    PQEntry output = heap[FRONT_INDEX];
    heap[FRONT_INDEX] = heap[theSize];
    int currIndex = 1;
    while (currIndex*TWO < theSize) {
        int childIndex;
        if (heap[currIndex * TWO].priority == heap[(currIndex * TWO) + 1].priority) {     // if the two children have equal prioroity
            (heap[currIndex * TWO].value < heap[(currIndex * TWO) + 1].value) ? childIndex = currIndex * TWO: childIndex = (currIndex * TWO) + 1; // makes childIndex the higher priority child's index
        }
        else {      // if the two children have different priorities
            (heap[currIndex * TWO].priority < heap[(currIndex * TWO) + 1].priority) ? childIndex = currIndex * TWO: childIndex = (currIndex * TWO) + 1; // makes childIndex the higher priority child's index
        }

        if (heap[currIndex].priority >= heap[childIndex].priority) {
            if (heap[currIndex].priority == heap[childIndex].priority) {
                if (heap[currIndex].value > heap[childIndex].value) {
                    PQEntry temp = heap[currIndex];
                    heap[currIndex] = heap[childIndex];
                    heap[childIndex] = temp;
                    currIndex = childIndex;
                }
                else {
                    break;
                }
            }
            else {
                PQEntry temp = heap[currIndex];
                heap[currIndex] = heap[childIndex];
                heap[childIndex] = temp;
                currIndex = childIndex;
            }
        }
        else {
            break;
        }
    }
    theSize--;
    return output.value;
}

// O(logN)
void HeapPriorityQueue::enqueue(string value, int priority) {
    theSize++;
    resize();
    heap[theSize] = PQEntry(value, priority);
    int current = theSize;
    bubbleUp(current);
}

// O(1)
bool HeapPriorityQueue::isEmpty() const {
    return (theSize == 0);
}

// O(1)
string HeapPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    return heap[1].value;
}

// O(1)
int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    return heap[FRONT_INDEX].priority;
}

// O(1)
int HeapPriorityQueue::size() const {
    return theSize;
}

// O(N)
void HeapPriorityQueue::resize() {
    if (theSize == capacity) {
        PQEntry* newArray = new PQEntry[capacity * TWO];
        for (int i = 0; i < theSize; i++) {
            newArray[i] = heap[i];
        }
        delete[] heap;
        heap = newArray;
        capacity *= TWO;
    }
}

//O(logN)
void HeapPriorityQueue::bubbleUp(int current) {
    while (current != FRONT_INDEX) {
        if (heap[current/TWO].priority >= heap[current].priority) {    // if parent has lower priority than child
            if (heap[current/TWO].priority == heap[current].priority) {
                if (heap[current/TWO].value >= heap[current].value) {
                    PQEntry temp = heap[current/TWO];
                    heap[current/TWO] = heap[current];
                    heap[current] = temp;
                    current /= TWO;
                }
                else {
                    break;
                }
            }
            else {
                PQEntry temp = heap[current/TWO];
                heap[current/TWO] = heap[current];
                heap[current] = temp;
                current /= TWO;
            }
        }
        else {
            break;
        }
    }
}

// O(N)
ostream& operator <<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int i = 1; i <= queue.theSize; i++) {
        out << queue.heap[i];
        if (i != queue.theSize) {
            out <<", ";
        }
    }
    out << "}";
    return out;
}

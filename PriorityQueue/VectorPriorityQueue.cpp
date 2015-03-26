/* Shan Rajapakshe
 Priority queue implemented as an unsorted vector

 Generally easy to create and comprehend. Quick to enqueue elements, as it is unsorted. However
 it is very slow when checking for the highest priority element, dequeieng, etc
*/

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    // VECTOR WILL USE IT'S OWN CONSTRUCTOR
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // VECTOR WILL USE ITS OWN DESTRUCTOR
}

// O(N)
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < priorityQueue.size(); i++) {
        if (priorityQueue[i].value == value) {
            priorityQueue[i].priority = newPriority;
            break;
        }
    }
}

// O(1)
void VectorPriorityQueue::clear() {
    priorityQueue.clear();
}

// O(N)
string VectorPriorityQueue::dequeue() {     // similar to peek!
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    int index = priorityLooper();
    PQEntry output = priorityQueue[index];
    priorityQueue.remove(index);
    return output.value;
}

// O(1)
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry next(value, priority);
    priorityQueue.add(next);
}

// O(1)
bool VectorPriorityQueue::isEmpty() const {
    return (priorityQueue.size() == 0);
}

// O(N)
string VectorPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    PQEntry output = priorityQueue[priorityLooper()];
    return output.value;
}

// O(N)
int VectorPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    int index = priorityLooper();
    return priorityQueue[index].priority;
}

// O(1)
int VectorPriorityQueue::size() const {
    return priorityQueue.size();
}

// O(N)
int VectorPriorityQueue::priorityLooper() const {
    int thePriorityIndex = 0;
    int thePriority = 999;
    for (int i = 0; i < priorityQueue.size(); i++) {
        if ( thePriority >= priorityQueue[i].priority) {
            if (thePriority == priorityQueue[i].priority) {
                if (priorityQueue[thePriorityIndex].value > priorityQueue[i].value) {
                    thePriority = priorityQueue[i].priority;
                    thePriorityIndex = i;
                }
            }
            else {
                thePriority = priorityQueue[i].priority;
                thePriorityIndex = i;
            }
        }
    }
    return thePriorityIndex;
}

// O(N)
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for (int i = 0; i < queue.size(); i++) {
        out << queue.priorityQueue[i];
        if (i != queue.size() - 1) {
           out << ", ";
        }
    }
    out << "}";
    return out;
}

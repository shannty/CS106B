/* Shan Rajapakshe
Priority Queue implemented as a sorted linked list

Each element in the list is preceeded with one of a higher priority and followed by one of a lower priority.
Quick to dequeue, peek, peekPriority as the front element always has the highest priority. However, enqueuing
may require shuffling around the entire queue, so it is very inefficient.

Also, the use of pointers requires a bit more mental gymnastics than usual
*/

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

// O(N)
LinkedPriorityQueue::~LinkedPriorityQueue() {
    ListNode* curr = front;
    while (curr != NULL){
        ListNode* temp = curr->next;
        delete curr;
        curr = temp;
    }
}

// O(N)
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* curr = front;
    ListNode* prev = front;
    while (curr != NULL) {
        if (value == curr->value) {    // when the correct node is found
            if (newPriority < curr->priority) {
                prev->next = curr->next;
                delete curr;
                enqueue(value, newPriority);
                break;
            }
            else {
                throw "That is not a higher priority than the previous one!";
            }
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
    if (curr == NULL) {
        throw "Value not found in queue!";
    }
}

// O(N)
void LinkedPriorityQueue::clear() {
    ListNode* curr = front;
    while (curr != NULL){
        ListNode* temp = curr->next;
        if (curr == front) {
            front = NULL;
        }
        else {
            delete curr;
        }
        curr = temp;
    }
}

// O(1)
string LinkedPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    ListNode* temp = front;
    front = front->next;
    return temp->value;
    delete temp;
}

// O(N)
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* curr = front;
    ListNode* prev = front;
    if (isEmpty()) {
        front = new ListNode (value, priority);
    }
    else {
        while (curr != NULL) {    // while current is not the last listNode
            if (curr->priority >= priority) {
                if (curr->priority == priority) {  // if the priorities are the same;
                    if (curr->value > value) {
                        if (front == curr) {
                            front = new ListNode(value, priority, front);
                            break;
                        }
                        else {  // if not front
                            prev->next = new ListNode (value, priority, curr);
                            break;
                        }
                    }
                    else {  // move to next list node
                        prev = curr;
                        curr = curr->next;
                    }
                }
                else {
                    if (front == curr) {    // if inserting at very front of linked list
                        front = new ListNode(value, priority, front);
                        break;
                    }
                    else {  // inserting between prev and curr
                        prev->next = new ListNode (value, priority, curr);
                        break;
                    }
                }
            }
            else {  // move to check next ListNode
                prev = curr;
                curr = curr->next;
            }
        }
        if (curr == NULL) {
            prev ->next = new ListNode(value, priority);    // inserting at last position
        }
    }
}

// O(1)
bool LinkedPriorityQueue::isEmpty() const {
    return (front == NULL);
}

// O(1)
string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    return front->value;
}


// O(1)
int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "The queue does not contain any elements!";
    }
    return front->priority;
}

// O(N)
int LinkedPriorityQueue::size() const {
    if (isEmpty()) {
        return 0;
    }
    ListNode* curr = front;
    int theSize = 0;
    while (curr != NULL) {
        curr = curr->next;
        theSize++;
    }
    return theSize;
}

// O(N)
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* curr = queue.front;
    if (!queue.isEmpty()) {
        while (curr != NULL){
            out << *curr;
            if (curr->next != NULL) {
                out << ", ";
            }
            curr = curr->next;
        }
    }
    out << "}";
    return out;
}

/*
 * Shan Rajapakshe
 * Trail Blazer: Provides the code for graph algorithms to be used in creating and traversing
 * mazes.
 *
 * Note: Got help from Dr. Lee with speeding up kruskal's algorithm (switched from hashsets
 * to hashset pointers as cluster implementation).
 */

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "hashset.h"
#include "map.h"
using namespace std;

const int TWO = 2;
const int ZERO = 0;

/*
 * bool depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path): Recursive helper
 * for depthFirstSearch().
 */
bool depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    start->visited = true;
    path += start;
    start->setColor(GREEN);
    if (start == end) {
        return true;
    }
    for (Vertex* neighbor: graph.getNeighbors(start)) {     // for all neighbors of start
        if (neighbor->visited != true) {                    // if the neighbor has not been visited
            neighbor->setColor(YELLOW);
            if (depthFirstSearch(graph, neighbor, end, path)) {
                neighbor->visited = false;     // resets start to false in preparation for next search
                return true;
            }
        }
    }
    path.remove(path.size() - 1);
    start->visited = false;
    start->setColor(GRAY);
    return false;
}

/*
 * Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end): Performs a depth first search.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    depthFirstSearch(graph, start, end, path);
    return path;
}

/*
 * void getPathAndReset(Vector<Vertex*> path, BasicGraph& graph): Helper function that gets the path, and resets
 * the visited parameter of each vertex.
 * Called by dijkstraStarHelper() and breadthFirstSearch().
 */
void getPathAndReset(Vector<Vertex*> &path, BasicGraph& graph, Vertex* current) {
    Vertex* thePrev = current;
    while (thePrev != NULL) {
        path.insert(ZERO, thePrev);
        thePrev = thePrev->previous;
    }
    for (Vertex* currRemove: graph.getNodeSet()) {
        currRemove->visited = false;
        currRemove->previous = NULL;
    }
}

/*
 * Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end): Performs a depth first search.
 * Makes a call to getPathAndReset().
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    Queue<Vertex*> theQueue;
    theQueue.enqueue(start);
    start->visited = true;
    while (!theQueue.isEmpty()) {
        Vertex* current = theQueue.dequeue();
        current->setColor(GREEN);
        if (current == end) {
            getPathAndReset(path, graph, current);
            break;
        }
        for (Vertex* neighbor: graph.getNeighbors(current)) {
            if (neighbor->visited != true) {
                neighbor->visited = true;
                neighbor->previous = current;
                neighbor->setColor(YELLOW);
                theQueue.enqueue(neighbor);
            }
        }
    }
    return path;
}

/*
 * Vector<Vertex*> dijkstraStarHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool choice): Helper that performs
 * the common calculations of dijkstrasAlgorithm() and aStar(). Bool choice determines if which is to be performed.
 * Makes a call to getPathAndReset().
 * bool choice: true = aStar()
 * bool choice: false = dijkstrasAlgorithm()
 */
Vector<Vertex*> dijkstraStarHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool choice) {
    for (Vertex* current: graph.getVertexSet()) {
        current->cost = POSITIVE_INFINITY;
    }
    start->cost = ZERO;
    PriorityQueue<Vertex*> pqueue;
    if (choice) {                                           // if AStar, use heuristic function
        pqueue.enqueue(start, heuristicFunction(start, end));
    }
    else {                                                  // if dijkstra's, use regular cost
        pqueue.enqueue(start, start->cost);
    }
    start->setColor(GREEN);
    while (!pqueue.isEmpty()) {                             // while pqueue is not empty
        Vertex* current = pqueue.dequeue();
        current->visited = true;
        current->setColor(GREEN);
        if (current == end) {
            break;
        }
        for (Vertex* neighbor: graph.getNeighbors(current)) {
            double theCost = current->cost + graph.getEdge(current, neighbor)->cost;
            if (theCost < neighbor->cost) {
                neighbor->visited = true;
                neighbor->cost = theCost;
                neighbor->previous = current;
                if (choice) {                               // if AStar, use heuristic function
                    pqueue.enqueue(neighbor, neighbor->cost + heuristicFunction(neighbor, end));
                }
                else {                                      // if dijkstra's, use regular cost
                    pqueue.enqueue(neighbor, neighbor->cost);
                }
                neighbor->setColor(YELLOW);
            }
        }
    }
    Vector<Vertex*> path;
    getPathAndReset(path, graph, end);
    return path;
}

/*
 * Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end): Uses dijkstra's alogrithm to
 * determine a path. Makes a call to dijkstraStarHelper() with choice equal to false.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    bool choice = false;
    return dijkstraStarHelper(graph, start, end, choice);
}

/*
 * Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end): Uses the A star algorithm to determine a math.
 * Makes a call to dijkstraStarHelper() with choice equal to true.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    bool choice = true;
    return dijkstraStarHelper(graph, start, end, choice);
}


/*
 * Set<Edge*> kruskal(BasicGraph& graph): Algorithm that generates a random maze by creating a
 * minimum spanning tree with kruskal's algorithm.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Map<Vertex*, HashSet<Vertex*>* > clusters;
    PriorityQueue<Edge*> pqueue;

    for (Vertex* currVertex: graph.getVertexSet()) {        // puts every vertex into its own cluster (a hashset of vertexs)
        HashSet<Vertex*>* currCluster = new HashSet<Vertex*>;
        *currCluster += currVertex;
        clusters.put(currVertex, currCluster);
    }
    for (Edge* currEdge: graph.getEdgeSet()) {              // places all edges in priority queue
        pqueue.enqueue(currEdge, currEdge->cost);
    }
    int uniqueValues = clusters.values().size();
    while (uniqueValues >= TWO) {
        Edge* currEdge = pqueue.dequeue();
        if (clusters[currEdge->start] != clusters[currEdge->finish]) {      // if start and finish not in same cluster
            HashSet<Vertex*>* combined = clusters[currEdge->start];
            (*combined)+= *(clusters[currEdge->finish]);                    // combine them
            delete clusters[currEdge->finish];                              // delete pointer to finish
            for (Vertex* curr: *combined) {                                 // loops through combined, updating the values of clusters as necessary
                if (clusters.containsKey(curr)) {
                    clusters[curr] = combined;
                }
            }
            mst +=currEdge;                                                 // adds currEdge to mst
            uniqueValues--;
        }
    }
    return mst;
}

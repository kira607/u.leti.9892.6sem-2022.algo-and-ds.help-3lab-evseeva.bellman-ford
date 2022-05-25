#include "graph.h"

int main(){
    Graph graph;

    if(!loadGraph(graph, "../data/data_1.txt"))
        return 1;

    graph.printVertices(); std::cout << '\n';
    graph.print(); std::cout << '\n';
    printShortestPath(graph, "A", "F"); std::cout << '\n';

    return 0;
}
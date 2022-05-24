#include "graph.h"


bool load_graph(Graph &graph, const std::string &path)
{
    try {
        graph.load("../cities.txt");
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return false;
    }
    return true;
}


int main(){
    Graph graph;

    if(!load_graph(graph, "../cities.txt"))
        return 1;

    graph.printNodes();
    graph.printMatrix();
    return 0;
}
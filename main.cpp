#include "graph.h"


int main(){
    Graph graph;
    graph.load("/home/kirill/programming/3lab-evs/cities.txt");
    graph.print();
    return 0;
}
#ifndef INC_GRAPH_H
#define INC_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>


#define MATRIX(_T) std::vector<std::vector<_T>>


class Graph
{
public:
    Graph();
    void load(const std::string &path);
    void print();
private:
    std::map<std::string, int> _nodes;
    MATRIX(int) _matrix;
    int _getNodeIndex(const std::string &key);
    void _createMatrx(MATRIX(std::string) lines);
};

#endif //INC_GRAPH_H
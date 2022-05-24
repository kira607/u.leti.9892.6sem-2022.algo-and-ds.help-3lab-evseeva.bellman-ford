#ifndef INC_GRAPH_H
#define INC_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#define MATRIX(_T) std::vector<std::vector<_T>>
#define LOG(msg) do {std::cout << msg;} while(0)
typedef std::string Key;
typedef const std::string & KeyRef;
typedef std::ifstream File;


class Graph
{
public:
    Graph();
    void load(const std::string &path);
    void printMatrix() const;
    void printNodes() const;
    int size() const;
    std::vector<Key> get_nodes() const;
private:
    std::map<Key, int> _nodes;
    MATRIX(int) _matrix;
    File _file;
    MATRIX(Key) _lines;

    void _openFile(const std::string &path);
    void _loadLines();
    void _closeFile();

    void _makeNode(KeyRef key);
    void _makeNodes();
    void _createMatrix();

    int _getNodeIndex(KeyRef key) const;
    Key _getKey(int index) const;
};

#endif //INC_GRAPH_H
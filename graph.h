#ifndef INC_GRAPH_H
#define INC_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>


#define MATRIX(_T) std::vector<std::vector<_T>>
#define LOG(msg) do {std::cout << msg;} while(0)
#define INF INT_MAX
typedef std::string TKey;
typedef std::tuple<int, int, int> TEdge;
typedef const std::string & TKeyRef;
typedef std::ifstream TFile;
typedef std::pair<std::vector<TKey>, int> TShortestPath;


class Graph
{
public:
    Graph();
    void load(const std::string &path);
    void print() const;
    void printVertices() const;
    int sizeVertices() const;
    int sizeEdges() const;
    std::vector<TKey> getVertices() const;
    std::vector<TEdge> getEdges() const;
    int findShortestPath(TKeyRef start, TKeyRef end) const;

private:
    TFile _file;
    std::map<TKey, int> _vertices;
    MATRIX(int) _matrix;
    MATRIX(TKey) _lines;

    void _openFile(const std::string &path);
    void _loadLines();
    void _closeFile();

    void _addVertex(TKeyRef key);
    void _makeVertices();
    void _createMatrix();

    int _getVertexIndex(TKeyRef key) const;
    TKey _getVertexName(int index) const;

    TEdge _getEdge(TKeyRef start, TKeyRef end) const;
    TEdge _getEdge(TKeyRef start, int end) const;
    TEdge _getEdge(int start, TKeyRef end) const;
    TEdge _getEdge(int start, int end) const;

    std::vector<int> _bellmanFordClassic(int start) const;
    void _bellmanFordFull(int start) const;
};


bool loadGraph(Graph &graph, const std::string &path);
void printShortestPath(const Graph &g,TKeyRef start, TKeyRef end);

template<typename T>
void printRow(std::vector<T> v, const std::string &sep = " ", const std::string &end = "\n");

template<typename T>
void printMatrix(MATRIX(T) m, const std::string &name = "?");

template<typename T>
std::vector<T> makeRow(int l, T v = nullptr);

template<typename T>
MATRIX(T) makeMatrix(int m, int n = 0, T v = nullptr);

#endif //INC_GRAPH_H
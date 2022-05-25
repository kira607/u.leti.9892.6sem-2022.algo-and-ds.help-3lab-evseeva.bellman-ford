#include "graph.h"

bool loadGraph(Graph &graph, const std::string &path)
{
    try {
        graph.load(path);
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return false;
    }
    return true;
}

void printShortestPath(const Graph &g,TKeyRef start, TKeyRef end)
{
    auto shortest_path = g.findShortestPath(start, end);
    std::cout << "Shortest path from \"" << start << "\" to \"" << end << "\":\n";
    // for(const auto &city : shortest_path.first)
    // {
    //     std::cout << city;
    //     if(city != shortest_path.first.back())
    //     {
    //         std::cout << " -> ";
    //     }
    // }
    if(shortest_path == INF)
    {
        std::cout << "Could not be found. Negative cycle.\n";
    }
    else
    {
        std::cout << "The price is: " << shortest_path << '\n';
    }
}

template<typename T>
void printRow(std::vector<T> v, const std::string &sep, const std::string &end)
{
    for(const auto &elem : v)
    {
        LOG(elem << sep);
    }
    LOG(end);
}

template<typename T>
void printMatrix(MATRIX(T) m, const std::string &name)
{
    LOG("Matrix " << name << " (" << m.size() << 'x' << m.at(0).size() << "):\n");
    for(const auto& line : m)
    {
        printRow<T>(line);
    }
}

template<typename T>
std::vector<T> makeRow(int l, T v)
{
    if(!v) v = T{};
    std::vector<T> row;
    for(int i = 0; i < l; ++i)
    {
        row.push_back(v);
    }
    return row;
}

template<typename T>
MATRIX(T) makeMatrix(int m, int n, T v)
{
    MATRIX(T) M;
    if(!v) v = T{};
    if(!n) n = m;
    for(int i = 0; i < m; ++i)
    {
        M.push_back(makeRow(n, v));
    }
    return M;
}


/* ===================== GRAPH PUBLIC ===================== */

Graph::Graph() = default;

void Graph::load(const std::string &path)
{
    _openFile(path);
    _loadLines();
    _closeFile();

    _makeVertices();
    _createMatrix();
}

void Graph::print() const
{
    int index = 0;
    LOG("Graph matrix (" << sizeVertices() << 'x' << sizeVertices() << "):\n");
    for(const auto& line : _matrix)
    {
        LOG(_getVertexName(index) << ' ');
        for(auto digit : line)
        {
            LOG(digit << ' ');
        }
        LOG('\n');
        ++index;
    }
}

void Graph::printVertices() const
{
    LOG("Nodes (count: " << sizeVertices() << "):\n");
    int num = 1;
    for(const auto &node: getVertices())
    {
        LOG('[' << num << "]: " << node << '\n');
        ++num;
    }
}

int Graph::sizeVertices() const
{
    return static_cast<int>(_vertices.size());
}

int Graph::sizeEdges() const
{
    int edges = 0;
    for(const auto &row : _matrix)
    {
        for(const auto &weight : row)
        {
            if(weight != 0)
            {
                ++edges;
            }
        }
    }
    return edges;
}

std::vector<TKey> Graph::getVertices() const
{
    std::vector<TKey> nds;
    for(const auto& n : _vertices)
    {
        nds.push_back(n.first);
    }
    return nds;
}

std::vector<TEdge> Graph::getEdges() const
{
    std::vector<TEdge> edges;
    for(int i = 0; i < sizeVertices(); ++i)
    {
        for(int j = 0; j < sizeVertices(); ++j)
        {
            auto e = _getEdge(i, j);
            if(std::get<2>(e) != 0)
            {
                edges.push_back(e);
            }
        }
    }
    return edges;
}

int Graph::findShortestPath(TKeyRef start, TKeyRef end) const
{
    auto distances = _bellmanFordClassic(_getVertexIndex(start));
    if(distances.empty()) return INF;  // negative cycle
    return distances[_getVertexIndex(end)];
}


/* ===================== GRAPH PRIVATE ===================== */

void Graph::_openFile(const std::string &path)
{
    _file.open(path);
    if(_file.is_open()) return;

    std::stringstream ss;
    ss << "Could not open file: " << path;
    throw std::runtime_error(ss.str());
}

void Graph::_loadLines()
{
    TKey line, word;
    while(std::getline(_file, line))
    {
        std::stringstream ss(line);
        std::vector<TKey> l;
        while(std::getline(ss, word, ';'))
        {
            l.push_back(word);
        }
        _lines.push_back(l);
    }
}

void Graph::_closeFile()
{
    _file.close();
}

void Graph::_addVertex(TKeyRef key) {
    auto node = _vertices.find(key);
    if(node != _vertices.end())
    {
        return;
    }
    int new_index = static_cast<int>(_vertices.size());
    _vertices.insert({key, new_index});
}

void Graph::_makeVertices()
{
    int line_num = 1;
    for(auto line : _lines)
    {
        if(line.size() != 4)
        {
            std::stringstream ss;
            ss << "TFile loading error: too " <<
            (line.size() < 4 ? "few" : "much") <<
            " values on line " << line_num << '.';
            throw std::runtime_error(ss.str());
        }
        auto a = line.at(0);
        auto b = line.at(1);
        _addVertex(a);
        _addVertex(b);
        ++line_num;
    }
}

void Graph::_createMatrix()
{
    _matrix.resize(sizeVertices());
    for(auto &row : _matrix)
    {
        row.resize(sizeVertices());
    }

    for(auto line : _lines)
    {
        int row = _getVertexIndex(line.at(0));
        int col = _getVertexIndex(line.at(1));
        auto cost_a_b_string = line.at(2);
        auto cost_b_a_string = line.at(3);

        if(cost_a_b_string != "N/A")
        {
            int cost_a_b = std::stoi(cost_a_b_string);
            _matrix.at(row).at(col) = cost_a_b;
        }

        if(cost_b_a_string != "N/A")
        {
            int cost_b_a = std::stoi(cost_b_a_string);
            _matrix.at(col).at(row) = cost_b_a;
        }
    }
}

int Graph::_getVertexIndex(TKeyRef key) const
{
    auto node = _vertices.find(key);
    if(node != _vertices.end())
    {
        return node->second;
    }
    return -1;
}

TKey Graph::_getVertexName(int index) const
{
    for(const auto &pair : _vertices)
    {
        if(pair.second == index)
        {
            return pair.first;
        }
    }
    std::stringstream ss;
    ss << "Unknown index: " << index;
    throw std::runtime_error(ss.str());
}

TEdge Graph::_getEdge(TKeyRef start, TKeyRef end) const
{return _getEdge(_getVertexIndex(start), _getVertexIndex(end));}

TEdge Graph::_getEdge(TKeyRef start, int end) const
{return _getEdge(_getVertexIndex(start), end);}

TEdge Graph::_getEdge(int start, TKeyRef end) const
{return _getEdge(start, _getVertexIndex(end));}

TEdge Graph::_getEdge(int start, int end) const
{return std::make_tuple(start, end, _matrix.at(start).at(end));}

std::vector<int> Graph::_bellmanFordClassic(int start) const
{
    int vertices = sizeVertices();
    auto edges = getEdges();
    std::vector<int> dist = makeRow<int>(vertices, INF);
    dist[start] = 0;

    for(int i = 1; i <= vertices - 1; ++i)
    {
        for(auto &e : edges)
        {
            int src, dest, weight; std::tie(src, dest, weight) = e;
            int nw = dist[src] + weight;
            if (dist[src] != INF && dist[dest] > nw)
            {
                dist[dest] = nw;
            }
        }
    }

    for(auto &e : edges)
    {
        int src, dest, weight; std::tie(src, dest, weight) = e;
        int nw = dist[src] + weight;
        if (dist[src] != INF && dist[dest] > nw)
        {
            // negative cycle
            return std::vector<int>{};
        }
    }

    // printRow(dist);

    return dist;
}

void Graph::_bellmanFordFull(int start) const
{
    /// NOT WORKING...
    int vertices = sizeVertices();
    auto edges = getEdges();
    MATRIX(int) A = makeMatrix<int>(vertices, vertices, INF);
    MATRIX(int) P = makeMatrix<int>(vertices, vertices, 0);
    for(auto &r : A) r[start] = 0;

    for(int i = 1; i < vertices; i++)
    {
        for(auto &e : edges)
        {
            int src, dest, weight; std::tie(src, dest, weight) = e;
            int nw = A[i-1][src] + weight;
            if (A[i-1][dest] != INF && A[i-1][dest] > nw)
            {
                A[i][dest] = nw;
            }
        }
    }

    int i = vertices - 1;
    for(auto &e : edges)
    {
        int src, dest, weight; std::tie(src, dest, weight) = e;
        int nw = A[src][i - 1] + weight;
        if (A[dest][i] != INF && A[dest][i] > nw)
        {
            std::cout << "NEGATIVE CYCLE\n";
        }
    }

    printMatrix<int>(A, "A");

    //return MATRIX(int){};
}
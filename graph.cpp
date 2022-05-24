#include "graph.h"

Graph::Graph() = default;

void Graph::load(const std::string &path)
{
    _openFile(path);
    _loadLines();
    _closeFile();

    _makeNodes();
    _createMatrix();
}

void Graph::printMatrix() const
{
    int index = 0;
    for(const auto& line : _matrix)
    {
        std::cout << _getKey(index) << ' ';
        for(auto digit : line)
        {
            std::cout << digit << ' ';
        }
        std::cout << '\n';
        ++index;
    }
}

void Graph::printNodes() const
{
    std::cout << "Nodes (count: " << this->size() << "):\n";
    int num = 1;
    for(const auto &node: this->get_nodes())
    {
        LOG('[' << num << "]: " << node << '\n');
        ++num;
    }
}

int Graph::size() const
{
    return static_cast<int>(_nodes.size());
}

std::vector<Key> Graph::get_nodes() const
{
    std::vector<Key> nds;
    for(const auto& n : _nodes)
    {
        nds.push_back(n.first);
    }
    return nds;
}


/* ===================== PRIVATE ===================== */

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
    Key line, word;
    while(std::getline(_file, line))
    {
        std::stringstream ss(line);
        std::vector<Key> l;
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

void Graph::_makeNode(KeyRef key) {
    auto node = _nodes.find(key);
    if(node != _nodes.end())
    {
        return;
    }
    int new_index = static_cast<int>(_nodes.size());
    _nodes.insert({key, new_index});
}

void Graph::_makeNodes()
{
    int line_num = 1;
    for(auto line : _lines)
    {
        if(line.size() != 4)
        {
            std::stringstream ss;
            ss << "File loading error: too " <<
            (line.size() < 4 ? "few" : "much") <<
            " values on line " << line_num << '.';
            throw std::runtime_error(ss.str());
        }
        auto a = line.at(0);
        auto b = line.at(1);
        _makeNode(a);
        _makeNode(b);
        ++line_num;
    }
}

void Graph::_createMatrix()
{
    _matrix.resize(this->size());
    for(auto &row : _matrix)
    {
        row.resize(this->size());
    }

    for(auto line : _lines)
    {
        int row = _getNodeIndex(line.at(0));
        int col = _getNodeIndex(line.at(1));
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

int Graph::_getNodeIndex(KeyRef key) const
{
    auto node = _nodes.find(key);
    if(node != _nodes.end())
    {
        return node->second;
    }
    return -1;
}

Key Graph::_getKey(int index) const
{
    for(const auto &pair : _nodes)
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


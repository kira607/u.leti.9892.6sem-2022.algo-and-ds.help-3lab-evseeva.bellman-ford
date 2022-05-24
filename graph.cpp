#include "graph.h"

Graph::Graph(){}

void Graph::load(const std::string &path)
{

    std::ifstream fin;
    fin.open(path);
    if(!fin.is_open()) 
    {
        return;
    }

    std::string line;
    MATRIX(std::string) lines;
    while(fin >> line)
    {
        std::stringstream l;
        l << line;
        std::string segment;
        std::vector<std::string> seglist;
        while(std::getline(l, segment, ';'))
        {
            seglist.push_back(segment);
        }
        lines.push_back(seglist);
    }
    _createMatrx(lines);
    fin.close();
}

void Graph::print()
{
    for(auto line : _matrix)
    {
        for(auto digit : line)
        {
            std::cout << digit << ' ';
        }
        std::cout << '\n';
    }
}

int Graph::_getNodeIndex(const std::string &key)
{
    auto node = _nodes.find(key);
    if(node != _nodes.end())
    {
        return node->second;
    }
    int new_index = _nodes.size();
    _nodes.insert({key, new_index});
    return new_index;
}

void Graph::_createMatrx(MATRIX(std::string) lines)
{
    for(auto line : lines)
    {
        auto a = line.at(0);
        auto b = line.at(1);
        auto cost_a_b_string = line.at(2);
        auto cost_b_a_string = line.at(3);
        // std::stoi(string)
        
    }
}

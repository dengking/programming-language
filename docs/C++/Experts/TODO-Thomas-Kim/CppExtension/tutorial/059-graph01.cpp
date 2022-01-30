#include "059-graph01.hpp"

tpf::sstream stream;
auto endl = tpf::endl;

using graph_t = tpf::graph_t<const char*>;

void test_graph_data_structure()
{
    graph_t g;

    g.emplace_back("Tom", 1, 3);
    g.emplace_back("Sophia", 0, 2);
    g.emplace_back("Amie", 1, 3, 4);
    g.emplace_back("Albert", 0, 2, 4);
    g.emplace_back("Michelle", 2, 3);

    stream <<"graph node list - \n"
        << g << endl;

    size_t size = g.size();
    stream << "List adjacent nodes" << endl;
    for(size_t index = 0; index < size; ++index)
    {
        stream << g[index] << endl;
    }

    stream << endl;
    stream << "List node information" << endl;
    for(size_t index = 0; index < size; ++index)
    {
        stream << g.node_info(index) << endl;
    }

    stream << endl;
    stream << "List node value and adjacent node list" << endl;
    for(size_t index = 0; index < size; ++index)
    {
        stream << g.node_value(index) << " - " 
                << g.adjacency_node_list(index) << endl;
    }

    stream << endl;
    stream << "List node value and adjacent node values" << endl;
    for(size_t index = 0; index < size; ++index)
    {
        stream << g.node_value(index)
            << " - " << g.adjacent_node_values(index) << endl;
    }

    stream << endl;
    stream << "Accessing each individual nodes" << endl;
    for(size_t index = 0; index < size; ++index)
    {
        auto [value, adjacency_list] = g[index];

        stream << "Node value: " << value 
            <<" - Adjacency list: " << adjacency_list << endl;
    }
}

int main()
{
    test_graph_data_structure();
}
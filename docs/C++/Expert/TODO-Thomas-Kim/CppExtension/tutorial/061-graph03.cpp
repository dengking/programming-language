#include <tpf_graph.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto endL = tpf::endL;
auto nl = tpf::nl;

// Graph Data Structure 4. Dijkstra’s Shortest Path Algorithm
// https://youtu.be/pVfj6mxhdMw
void test_Dijkstra_shortest_path_by_Computer_Science()
{
    using graph = tpf::graph::graph_t<tpf::graph::edge_direction::undirected_weighted,
        tpf::graph::edge_plurality::single_edged, const char*>;

    using visit_mode = graph::visit_mode;
    using path_info = graph::path_info;

    graph g;
        
    try
    {
        g.emplace_back_node("A", 1, 3);
        g.emplace_back_node("B", 0, 2, 3, 4);
        g.emplace_back_node("C", 1, 4);
        g.emplace_back_node("D", 0, 1, 4);
        g.emplace_back_node("E", 3, 1, 2);
  
        g.integrity_check_nodes_throws();

        g.emplace_back_edge(0, 1, 6);
        g.emplace_back_edge(0, 3, 1);

        g.emplace_back_edge(1, 0, 6);
        g.emplace_back_edge(1, 2, 5);
        g.emplace_back_edge(1, 3, 2);
        g.emplace_back_edge(1, 4, 2);

        g.emplace_back_edge(2, 1, 5);
        g.emplace_back_edge(2, 4, 5);

        g.emplace_back_edge(3, 0, 1);
        g.emplace_back_edge(3, 1, 2);
        g.emplace_back_edge(3, 4, 1);   
        
        g.emplace_back_edge(4, 1, 2);
        g.emplace_back_edge(4, 2, 5);
        
    }
    catch(const std::exception& e)
    {
        stream << e << endl;
        return;
    }

    stream << g.shortest_path_report("A", "C") << endl;

}

// Dijkstra's Algorithm by Nathaniel Fan
// https://www.youtube.com/watch?v=gdmfOwyQlcI
void test_dijksta_by_Nathaniel_Fan()
{
    using graph = tpf::graph::graph_t<tpf::graph::edge_direction::undirected_weighted,
        tpf::graph::edge_plurality::single_edged, const char*>;

    using visit_mode = graph::visit_mode;
    using path_info = graph::path_info;

    graph g;
        
    try
    {
        g.emplace_back_node("A", 1, 2, 4);
        g.emplace_back_node("B", 0, 2, 3);
        g.emplace_back_node("C", 0, 1, 3, 4);
        g.emplace_back_node("D", 1, 2, 4, 5, 6);
        g.emplace_back_node("E", 0, 2, 3, 6);
        g.emplace_back_node("F", 3, 6);
        g.emplace_back_node("G", 3, 4, 5);
  
        g.integrity_check_nodes_throws();

        g.emplace_back_edge(0, 1, 4);
        g.emplace_back_edge(0, 2, 3);
        g.emplace_back_edge(0, 4, 7);
        
        g.emplace_back_edge(1, 0, 4);
        g.emplace_back_edge(1, 2, 6);
        g.emplace_back_edge(1, 3, 5);

        g.emplace_back_edge(2, 0, 3);
        g.emplace_back_edge(2, 1, 6);
        g.emplace_back_edge(2, 3, 11);
        g.emplace_back_edge(2, 4, 8);

        g.emplace_back_edge(3, 1, 5);
        g.emplace_back_edge(3, 2, 11);
        g.emplace_back_edge(3, 4, 2);
        g.emplace_back_edge(3, 5, 2);
        g.emplace_back_edge(3, 6, 10);

        g.emplace_back_edge(4, 0, 7);
        g.emplace_back_edge(4, 2, 8);
        g.emplace_back_edge(4, 3, 2);
        g.emplace_back_edge(4, 6, 5);
        
        g.emplace_back_edge(5, 3, 2);
        g.emplace_back_edge(5, 6, 3);
        
        g.emplace_back_edge(6, 5, 3);
        g.emplace_back_edge(6, 3, 10);
        g.emplace_back_edge(6, 4, 5);
            
    }
    catch(const std::exception& e)
    {
        stream << e << endl;
        return;
    }

    stream << g.shortest_path_report("A", "G") << endl;

}

int main()
{
    // test_Dijkstra_shortest_path_by_Computer_Science();

    test_dijksta_by_Nathaniel_Fan();
}
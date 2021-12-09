#include "060-graph02.hpp"

tpf::sstream stream;
auto endl = tpf::endl;
auto nl = tpf::nl;

using graph_t = tpf::graph_t<int>;
using visit_mode = typename graph_t::visit_mode;

// In courtesy of 
// [Data Structure and Algorithm] Graph Search DFS, BFS Implmentation in Java
// https://www.youtube.com/watch?reload=9&v=_hxFgg7TLZQ&list=PL1_C6uWTeBDFgjmvxNA7Okb_UQtEOGNfy&index=25
void test_graph_graphic_presentation()
{
    graph_t g;

    g.emplace_back(0, 1);
    g.emplace_back(1, 0, 2, 3);
    g.emplace_back(2, 1, 3, 4);
    g.emplace_back(3, 1, 2, 4, 5);
    g.emplace_back(4, 2, 3);
    g.emplace_back(5, 3, 7, 6);
    g.emplace_back(6, 5, 8);
    g.emplace_back(7, 5);
    g.emplace_back(8, 6);

    stream << g.build_graph() << endl;
}

void test_graph_search_dfs_bfs()
{
    graph_t g;

    g.emplace_back(0, 1);
    g.emplace_back(1, 0, 2, 3);
    g.emplace_back(2, 1, 3, 4);
    g.emplace_back(3, 1, 2, 4, 5);
    g.emplace_back(4, 2, 3);
    g.emplace_back(5, 3, 7, 6);
    g.emplace_back(6, 5, 8);
    g.emplace_back(7, 5);
    g.emplace_back(8, 6);

// Breadth First Serach: { 0, 1, 2, 3, 4, 5, 6, 7, 8 }
// Depth First Serach  : { 1, 3, 5, 7, 6, 8, 4, 2, 0 }

    stream << "Breadth First Serach: "
        << g.traverse_bfs()<< endl;

    stream << "Depth First Serach: "
        << g.traverse_dfs()<< endl;

}

void test_graph_search_dfs_bfs_graphic_representation()
{
    graph_t g;

    g.emplace_back(0, 1);
    g.emplace_back(1, 0, 2, 3);
    g.emplace_back(2, 1, 3, 4);
    g.emplace_back(3, 1, 2, 4, 5);
    g.emplace_back(4, 2, 3);
    g.emplace_back(5, 3, 7, 6);
    g.emplace_back(6, 5, 8);
    g.emplace_back(7, 5);
    g.emplace_back(8, 6);

// Breadth First Serach: { 0, 1, 2, 3, 4, 5, 6, 7, 8 }
// Depth First Serach  : { 1, 3, 5, 7, 6, 8, 4, 2, 0 }

    stream << "Breadth First Serach: "<<endl;
    stream<< g.build_graph(visit_mode::visit_bfs)<< endl;

    stream << "Depth First Serach: "<<endl;
    stream<< g.build_graph(visit_mode::visit_dfs)<< endl;

}


int main()
{
    // test_graph_graphic_presentation();

    // test_graph_search_dfs_bfs();

    test_graph_search_dfs_bfs_graphic_representation();
}
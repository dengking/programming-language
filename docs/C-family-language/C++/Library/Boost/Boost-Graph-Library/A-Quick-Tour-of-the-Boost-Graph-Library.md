# [A Quick Tour of the Boost Graph Library](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/quick_tour.html)



![](D:/github/dengking/programming-language-theory/programming-language/docs/C-family-language/C++/Library/Boost/Boost-Graph-Library/analogy.gif)

## Constructing a Graph

[`adjacency_list`](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/adjacency_list.html) 

The `adjacency_list` class provides a generalized version of the classic "adjacency list" data structure. 

The first two template arguments (`vecS, vecS`) determine the data structure used to represent the **out-edges** for each vertex in the graph and the data structure used to represent the graph's **vertex set** (see section [Choosing the `Edgelist` and `VertexList`](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/using_adjacency_list.html#sec:choosing-graph-type) for information about the tradeoffs of the different data structures). 

The third argument, `bidirectionalS`, selects a directed graph that provides access to both out and in-edges. The other options for the third argument are `directedS` which selects a directed graph with only out-edges, and `undirectedS` which selects an undirected graph.

> NOTE: `bidirectionalS`、`directedS`、`undirectedS`都是第三个template parameter的argument。





## Accessing the Vertex Set



## Accessing the Edge Set



## The Adjacency Structure



### Vertex Descriptors

> NOTE: 在boost [New Iterator Concepts](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html)中，对它进行了介绍。

### Out-Edges, In-Edges, and Edge Descriptors


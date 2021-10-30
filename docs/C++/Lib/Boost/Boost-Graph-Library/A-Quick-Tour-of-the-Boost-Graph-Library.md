# [A Quick Tour of the Boost Graph Library](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/quick_tour.html)



![](./analogy.gif)

## Constructing a Graph

[`adjacency_list`](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/adjacency_list.html) 

The `adjacency_list` class provides a generalized version of the classic "adjacency list" data structure. 

The first two template arguments (`vecS, vecS`) determine the data structure used to represent the **out-edges** for each vertex in the graph and the data structure used to represent the graph's **vertex set** (see section [Choosing the `Edgelist` and `VertexList`](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/using_adjacency_list.html#sec:choosing-graph-type) for information about the tradeoffs of the different data structures). 

The third argument, `bidirectionalS`, selects a directed graph that provides access to both out and in-edges. The other options for the third argument are `directedS` which selects a directed graph with only out-edges, and `undirectedS` which selects an undirected graph.

> NOTE: `bidirectionalS`、`directedS`、`undirectedS`都是前三个template parameter的argument。

>  NOTE: 
>
> [MutableGraph](MutableGraph.html)
>
> [`add_edge()`](MutableGraph.html#sec:add-edge) 
>
> [`add_vertex()`](MutableGraph.html#sec:add-vertex)
>
> [`remove_vertex()`](MutableGraph.html#sec:remove-vertex) 
>
> 
>
> [edge iterator constructor](adjacency_list.html#sec:iterator-constructor) 



## Accessing the Vertex Set

> NOTE:
>
> [VertexListGraph](VertexListGraph.html) 
>
> [`vertices()`](VertexListGraph.html#sec:vertices) 

Note that different graph classes can have different associated vertex iterator types, which is why we need the `graph_traits` class. 

> NOTE: trait提供了一层抽象。



## Accessing the Edge Set

> NOTE: 
>
> [EdgeListGraph](EdgeListGraph.html) 
>
> [`edges()`](EdgeListGraph.html#sec:edges) 
>
> `source()`
>
> `target()` 



## The Adjacency Structure

In the next few examples we will explore the adjacency structure of the graph from the point of view of a particular vertex. We will look at the vertices' in-edges, out-edges, and its adjacent vertices. 

> NOTE: 前面两节：Accessing the Vertex Set、Accessing the Edge Set都是着眼于整个graph，即对整个graph进行操作，而本节开始，着眼于graph中的一个vertex，结合各种graph algorithm可知，各种algorithm都是基于vertex的。最最常见的便是获得一个vertex的adjacent vertexes。

We will encapsulate this in an "**exercise vertex**" function, and apply it to each vertex in the graph. To demonstrate the STL-interoperability of BGL, we will use the STL `for_each()` function to iterate through the vertices and apply the function.

```c++
  //...
  int main(int,char*[])
  {
    //...
    std::for_each(vertices(g).first, vertices(g).second,
                  exercise_vertex<Graph>(g));
    return 0;
  }
```

We use a functor for `exercise_vertex` instead of just a function because the graph object will be needed when we access information about each vertex; using a functor gives us a place to keep a reference to the graph object during the execution of the `std::for_each()`. Also we template the functor on the graph type so that it is reusable with different graph classes. Here is the start of the `exercise_vertex` functor:

> NOTE: functor VS function

```c++
  template <class Graph> struct exercise_vertex {
    exercise_vertex(Graph& g_) : g(g_) {}
    //...
    Graph& g;
  };
```



### Vertex Descriptors

> NOTE: 在boost [New Iterator Concepts](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html)中，对它进行了介绍。

The first thing we need to know in order to write the `operator()` method of the functor is the type for the vertex objects of the graph. The **vertex type** will be the parameter to the `operator()` method. To be precise, we do not deal with actual **vertex objects**, but rather with *vertex descriptors*. Many graph representations (such as adjacency lists) do not store actual **vertex objects**, while others do (e.g., pointer-linked graphs). This difference is hidden underneath the "black-box" of the **vertex descriptor object**. The vertex descriptor is something provided by each graph type that can be used to access information about the graph via the `out_edges()`, `in_edges()`, `adjacent_vertices()`, and property map functions that are described in the following sections. 

> NOTE: vertex descriptor是对vertex抽象描述，或者说它是一种协议。

The `vertex_descriptor` type is obtained through the `graph_traits` class. The `typename` keyword used below is necessary because the type on the left hand side of the scope `::` operator (the `graph_traits<Graph>` type) is dependent on a template parameter (the `Graph` type). Here is how we define the functor's apply method:

 

```c++
  template <class Graph> struct exercise_vertex {
    //...
    typedef typename graph_traits<Graph>
      ::vertex_descriptor Vertex;

    void operator()(const Vertex& v) const
    {
      //...
    }
    //...
  };
```

### Out-Edges, In-Edges, and Edge Descriptors

The out-edges of a vertex are accessed with the [`out_edges()`](IncidenceGraph.html#sec:out-edges) function of the [IncidenceGraph](IncidenceGraph.html) interface. The `out_edges()` function takes two arguments: the first argument is the vertex and the second is the graph object. The function returns a pair of iterators which provide access to all of the out-edges of a vertex (similar to how the `vertices()` function returned a pair of iterators). The iterators are called *out-edge iterators* and dereferencing one of these iterators gives an *edge descriptor* object. An edge descriptor plays the same kind of role as the **vertex descriptor object**, it is a "black box" provided by the graph type. The following code snippet prints the source-target pairs for each out-edge of vertex `v`.

```c++
  template <class Graph> struct exercise_vertex {
    //...
    void operator()(const Vertex& v) const
    {
      typedef graph_traits<Graph> GraphTraits;
      typename property_map<Graph, vertex_index_t>::type
        index = get(vertex_index, g);

      std::cout << "out-edges: ";
      typename GraphTraits::out_edge_iterator out_i, out_end;
      typename GraphTraits::edge_descriptor e;
      for (boost::tie(out_i, out_end) = out_edges(v, g);
           out_i != out_end; ++out_i) {
        e = *out_i;
        Vertex src = source(e, g), targ = target(e, g);
        std::cout << "(" << index[src] << ","
                  << index[targ] << ") ";
      }
      std::cout << std::endl;
      //...
    }
    //...
  };
```

The [`in_edges()`](BidirectionalGraph.html#sec:in-edges) function of the [BidirectionalGraph](BidirectionalGraph.html) interface provides access to all the in-edges of a vertex through *in-edge iterators*. The `in_edges()` function is only available for the `adjacency_list` if `bidirectionalS` is supplied for the `Directed` template parameter. There is an extra cost in space when `bidirectionalS` is specified instead of `directedS`.

 

```c++
  template <class Graph> struct exercise_vertex {
    //...
    void operator()(const Vertex& v) const
    {
      //...
      std::cout << "in-edges: ";
      typedef typename graph_traits<Graph> GraphTraits;
      typename GraphTraits::in_edge_iterator in_i, in_end;
      for (boost::tie(in_i, in_end) = in_edges(v,g);
           in_i != in_end; ++in_i) {
        e = *in_i;
        Vertex src = source(e, g), targ = target(e, g);
        std::cout << "(" << index[src] << "," << index[targ] << ") ";
      }
      std::cout << std::endl;
      //...
    }
    //...
  };
```



## Adding Some Color to your Graph

> NOTE: *internally stored property* and *externally stored property*

BGL uses a uniform mechanism to access both kinds of properties inside its graph algorithms called the *property map* interface, described in Section [Property Map Concepts](property_map.html). In addition, the [PropertyGraph](PropertyGraph.html) concept defines the interface for obtaining a **property map object** for an **internally stored property**.

The BGL `adjacency_list` class allows users to specify **internally stored properties** through plug-in template parameters of the graph class. How to do this is discussed in detail in Section [Internal Properties](using_adjacency_list.html#sec:adjacency-list-properties). **Externally stored properties** can be created in many different ways, although they are ultimately passed as separate arguments to the **graph algorithms**. One straightforward way to store properties is to create an **array** indexed by **vertex or edge index**. 

In the `adjacency_list` with `vecS`（vector） specified for the `VertexList` template parameter, vertices are automatically assigned indices, which can be accessed via the **property map** for the `vertex_index_t`. Edges are not automatically assigned indices. However the property mechanism can be used to attach indices to the edges which can be used to index into other externally stored properties.

> NOTE: 关于`vertex_index_t`，参见[Using `adjacency_list`#Internal Properties](file:///D:/github/boost/boost_1_73_0/libs/graph/doc/using_adjacency_list.html#sec:adjacency-list-properties)章节。

In the following example, we construct a graph and apply [`dijkstra_shortest_paths()`](dijkstra_shortest_paths.html). The complete source code for the example is in [`examples/dijkstra-example.cpp`](../example/dijkstra-example.cpp). Dijkstra's algorithm computes the shortest distance from the starting vertex to every other vertex in the graph.

Dijkstra's algorithm requires that a **weight property** is associated with each edge and a **distance property** with each vertex. Here we use an **internal property** for the weight and an **external property** for the distance. For the weight property we use the `property` class and specify `int` as the type used to represent weight values and `edge_weight_t` for the property tag (which is one of the BGL predefined property tags). The weight property is then used as a template argument for `adjacency_list`.

> NOTE: distance property所指的是source vertex到该vertex的distance，这是Dijkstar算法的最终输出。

The `listS` and `vecS` types are selectors that determine the data structure used inside the `adjacency_list` (see Section [Choosing the `Edgelist` and `VertexList`](using_adjacency_list.html#sec:choosing-graph-type)). The `directedS` type specifies that the graph should be directed (versus undirected). The following code shows the specification of the graph type and then the initialization of the graph. The edges and weights are passed to the graph constructor in the form of iterators (a pointer qualifies as a [RandomAccessIterator](http://www.boost.org/sgi/stl/RandomAccessIterator.html)).

 

```c++
  typedef adjacency_list<listS, vecS, directedS,
                         no_property, property<edge_weight_t, int> > Graph;
  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::pair<int,int> E;

  const int num_nodes = 5;
  E edges[] = { E(0,2),
                E(1,1), E(1,3), E(1,4),
                E(2,1), E(2,3),
                E(3,4),
                E(4,0), E(4,1) };
  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1};

  Graph G(edges, edges + sizeof(edges) / sizeof(E), weights, num_nodes);
```






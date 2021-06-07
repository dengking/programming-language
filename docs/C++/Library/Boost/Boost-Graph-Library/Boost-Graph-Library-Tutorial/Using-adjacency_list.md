# Using `adjacency_list`



## Choosing the `Edgelist` and `VertexList`



### Choosing the `VertexList` type

### Choosing the `OutEdgeList` type



## Directed and Undirected Adjacency Lists





## Internal Properties

The template parameters `VertexProperty` and `EdgeProperty` of the `adjacency_list` class are meant to be filled by these interior properties.

**NOTE**: The Boost Graph Library supports two interchangeable methods for specifying interior properties: [bundled properties](bundles.html) and property lists. The former is easier to use and requires less effort, whereas the latter is compatible with older, broken compilers and is backward-compatible with Boost versions prior to 1.32.0.

One may specify internal properties via **property lists**, which are build from instances of the property class declared as follows.



```c++
template <class PropertyTag, class T, class NextProperty = no_property>
struct property;
```

The [`PropertyTag`](./PropertyTag.html) template parameter is a tag class that simply identifies or gives a unique name to the property.

> NOTE: tag class是c++经常使用的技巧

The **`NextProperty`** parameter allows `property` types to be nested, so that an arbitrary number of properties can be attached to the same graph.

> NOTE: 这是一个技巧

### Custom Edge Properties

> NOTE: 本节所描述的是如何自定义edge property。
>
> 


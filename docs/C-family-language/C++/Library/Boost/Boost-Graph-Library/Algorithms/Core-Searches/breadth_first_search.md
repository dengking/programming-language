# `breadth_first_search`

BFS uses two data structures to to implement the traversal: a **color marker** for each vertex and a **queue**. **White vertices** are undiscovered while **gray vertices** are discovered but have undiscovered adjacent vertices. **Black vertices** are discovered and are adjacent to only other black or gray vertices. The algorithm proceeds by removing a vertex u from the queue and examining each out-edge *(u,v)*. If an adjacent vertex *v* is not already discovered, it is colored gray and placed in the queue. After all of the out-edges are examined, vertex *u* is colored black and the process is repeated. Pseudo-code for the BFS algorithm is a listed below.

```pseudocode
BFS(G, s)
  for each vertex u in V[G]
    color[u] := WHITE
    d[u] := infinity
    p[u] := u
  end for
  color[s] := GRAY
  d[s] := 0
  ENQUEUE(Q, s)
  while (Q != Ø)
    u := DEQUEUE(Q)
    for each vertex v in Adj[u]
      if (color[v] = WHITE)
        color[v] := GRAY
        d[v] := d[u] + 1
        p[v] := u
        ENQUEUE(Q, v)
      else
        if (color[v] = GRAY)
          ...
        elsek
          ...
    end for
    color[u] := BLACK
  end while
  return (d, p)
```



# `breadth_first_visit`
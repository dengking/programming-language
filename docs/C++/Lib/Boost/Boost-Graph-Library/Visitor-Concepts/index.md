# Visitor Concepts

| concept                      | explanation                                                  |
| ---------------------------- | ------------------------------------------------------------ |
| BFS Visitor Concept          | This concept defines the visitor interface for [`breadth_first_search()`](./breadth_first_search.html). Users can define a class with the BFS Visitor interface and pass and object of the class to `breadth_first_search()`, thereby augmenting the actions taken during the graph search. |
| DFS Visitor Concept          | This concept defines the visitor interface for [`depth_first_search()`](./depth_first_search.html). Users can define a class with the DFS Visitor interface and pass an object of the class to `depth_first_search()`, thereby augmenting the actions taken during the graph search. |
| Dijkstra Visitor Concept     | This concept defines the visitor interface for [`dijkstra_shortest_paths()`](./dijkstra_shortest_paths.html) and related algorithms. ...... |
| Bellman Ford Visitor Concept | This concept defines the visitor interface for [`bellman_ford_shortest_paths()`](./bellman_ford_shortest.html). ...... |
| AStar Visitor Concept        | This concept defines the visitor interface for [`astar_search()`](./astar_search.html). ...... |
| EventVisitor Concept         | This concept defines the interface for single-event visitors. An EventVisitor has an apply member function (`operator()`) which is invoked within the graph algorithm at the event-point specified by the `event_filter` typedef within the EventVisitor. EventVisitor's can be combined into an [EventVistorList](./EventVisitorList.html). |
| Planar Face Visitor Concept  | This concept defines the visitor interface for [`planar_face_traversal`](./planar_face_traversal.html)....... |
| TSP Tour Visitor concept     | This concept defines the visitor interface for [`metric_tsp_approx()`](./metric_tsp_approx.html) and related algorithms. |


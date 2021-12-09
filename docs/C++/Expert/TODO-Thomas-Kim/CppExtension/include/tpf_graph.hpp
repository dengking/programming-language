/**
 * @file tpf_graph.hpp
 * @author your name (you@domain.com)
 * @brief Implements set operations.
 * @version 0.1
 * @date 2019-07-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_GRAPH_HPP
#define _TPF_GRAPH_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <tpf_set.hpp>

#ifdef max
#define _TPF_GRAPH_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_GRAPH_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace  tpf 
{
    namespace graph
    {
        struct edge_direction
        {
            static constexpr int undirected = 0;            // 0000 0000
            static constexpr int undirected_unweighted = 0; // 0000 0000
            static constexpr int unweighted_undirected = 0; // 0000 0000
            
            static constexpr int directed = 1;              // 0000 0001
            static constexpr int directed_unweighted = 1;   // 0000 0001
            static constexpr int unweighted_directed = 1;   // 0000 0001
            
            static constexpr int weighted = 2;              // 0000 0010
            static constexpr int undirected_weighted = 2;   // 0000 0010
            static constexpr int weighted_undirected = 2;   // 0000 0010

            static constexpr int directed_weighted = 3;     // 0000 0011
            static constexpr int weighted_directed = 3;     // 0000 0011
        };

        constexpr bool is_directed(int direction)
        {
            return (direction & 1) != 0;
        }
        
        constexpr bool is_weighted(int weighted)
        {
            return (weighted & 2) != 0;
        }

        struct edge_plurality
        {
            static constexpr int single_edged = 0;
            static constexpr int plural_edged = 1;
            static constexpr int weighted_edge = 1;
        };

        template<int DirectedEdge, typename NodeIndexType, typename... Types> class edge;

        // undirected, directed, single edged (no weight)
        template<int DirectedEdge, typename EdgeNodeIndexType>
        class edge<DirectedEdge, EdgeNodeIndexType> 
        {
            template<int, int, typename, typename, typename, template<typename, typename...> class,
            template<typename, typename...> class, template<typename, typename...> class> friend class graph;

            public:

                using node_index_t = EdgeNodeIndexType;
                
                static constexpr int directed_edge = DirectedEdge;
                static constexpr node_index_t invalid_node_index = tpf::type_max_v<node_index_t>;           
                
                enum edge_index: size_t{ index_node_1, index_node_2 };
              
                using edge_tuple_t = std::tuple<node_index_t, node_index_t>;

            private:
                edge_tuple_t m_edge;

            public:

                edge() = default;

                template<typename IndexType_1, typename IndexType_2>
                edge(IndexType_1 index_1, IndexType_2 index_2, bool bForceDirection = false) noexcept
                {
                    set(index_1, index_2, bForceDirection);
                }

                node_index_t& node_1() noexcept{ return std::get<edge_index::index_node_1>(this->m_edge); }
                const node_index_t& node_1() const noexcept{ return std::get<edge_index::index_node_1>(this->m_edge); }
                
                node_index_t& node_2() noexcept { return std::get<edge_index::index_node_2>(this->m_edge); }
                const node_index_t& node_2() const noexcept { return std::get<edge_index::index_node_2>(this->m_edge); }
                            
                template<typename IndexType_1, typename IndexType_2>
                edge& set(IndexType_1 index_1, IndexType_2 index_2, bool bForceDirection = false) noexcept
                {
                    if(bForceDirection || is_directed(directed_edge))
                    {
                        this->node_1() = (node_index_t) index_1;
                        this->node_2() = (node_index_t) index_2;
                    }
                    else
                    {
                        this->node_1() = (node_index_t) tpf::minimum(index_1, index_2);
                        this->node_2() = (node_index_t) tpf::maximum(index_1, index_2);
                    }
                    
                    return *this;
                }

                template<typename IndexType>
                node_index_t other_node(IndexType node_index) const noexcept
                {
                    if(this->node_1() == (node_index_t)node_index)
                        return this->node_2();
                    else if(this->node_2() == (node_index_t)node_index)
                        return this->node_1();
                    else
                        return invalid_node_index;
                }

                static int compare_edges(const edge& lhs, const edge& rhs)
                {
                    if(lhs.node_1() == rhs.node_1())
                    {
                        if(lhs.node_2() == rhs.node_2())
                            return 0;
                        else if(lhs.node_2() < rhs.node_2())
                            return -1;
                        else // this->node_2() > rhs.node_2()
                            return 1;
                    }
                    else if(lhs.node_1() < rhs.node_1() )
                        return -1;
                    else // this->node_1() > rhs.node_1()
                        return 1;
                }

                struct weak_compare
                {
                    bool operator()(const edge& lhs, const edge& rhs) const
                    {
                        return compare_edges(lhs, rhs) < 0;
                    }
                };

                struct strong_compare
                {
                    bool operator()(const edge& lhs, const edge& rhs) const
                    {
                        return compare_edges(lhs, rhs) < 0;
                    }
                };

                inline static constexpr weak_compare weak_comparator{}; 
                inline static constexpr strong_compare strong_comparator{}; 

                friend bool operator < (const edge& lhs, const edge& rhs)
                {
                    return compare_edges(lhs, rhs) < 0;
                }

                friend bool operator > (const edge& lhs, const edge& rhs)
                {
                    return compare_edges(lhs, rhs) > 0;
                } 

                friend bool operator == (const edge& lhs, const edge& rhs)
                {
                    return compare_edges(lhs, rhs) == 0;
                }

                friend bool operator != (const edge& lhs, const edge& rhs)
                {
                    return  compare_edges(lhs, rhs) != 0;
                }

                void swap(edge& rhs) noexcept
                {
                    if(this != std::addressof(rhs))
                    {
                        this->m_edge.swap(rhs.m_edge);
                    }
                }
                
                friend void swap(edge& lhs, edge& rhs) noexcept
                {
                    lhs.swap(rhs);
                }

                edge(const edge&) = default;
                edge& operator = (const edge&) = default;

                edge_tuple_t& get() noexcept { return this->m_edge; }

                const edge_tuple_t& get() const noexcept { return this->m_edge; }

                friend tpf::sstream& operator << (tpf::sstream& os, const edge& e)
                {
                    os << "( "<<e.node_1()<< ", "<<e.node_2() << " )";

                    return os;
                }
        };

        // directed, undirected, singled edged, plural edged (weighted)
        template<int DirectedEdge, typename EdgeNodeIndexType, typename EdgeWeightType>
        class edge<DirectedEdge, EdgeNodeIndexType, EdgeWeightType> // Plural Edged, Weighted
        {
            template<int, int, typename, typename, typename, template<typename, typename...> class,
            template<typename, typename...> class, template<typename, typename...> class> friend class graph;

            public:

                using node_index_t = EdgeNodeIndexType;
                using weight_t = EdgeWeightType;

                static constexpr int directed_edge = DirectedEdge;
                static constexpr node_index_t invalid_node_index = tpf::type_max_v<node_index_t>;           
                
                enum edge_index: size_t{ index_node_1, index_node_2, index_weight };
              
                using edge_tuple_t = std::tuple<node_index_t, node_index_t, weight_t>;

            private:
                edge_tuple_t m_edge;

            public:

                edge() = default;

                template<typename IndexType_1, typename IndexType_2, typename WeightType>
                edge(IndexType_1 index_1, IndexType_2 index_2, WeightType weight, bool bForceDirection = false) noexcept
                { 
                    set(index_1, index_2, weight, bForceDirection);
                }

                weight_t& weight() noexcept { return std::get<edge_index::index_weight>(this->m_edge); };
                const weight_t& weight() const noexcept { return std::get<edge_index::index_weight>(this->m_edge); };
                
                node_index_t& node_1() noexcept{ return std::get<edge_index::index_node_1>(this->m_edge); }
                const node_index_t& node_1() const noexcept{ return std::get<edge_index::index_node_1>(this->m_edge); }
                
                node_index_t& node_2() noexcept { return std::get<edge_index::index_node_2>(this->m_edge); }
                const node_index_t& node_2() const noexcept { return std::get<edge_index::index_node_2>(this->m_edge); }
                            
                template<typename IndexType_1, typename IndexType_2, typename WeightType>
                edge& set(IndexType_1 index_1, IndexType_2 index_2, WeightType weight, bool bForceDirection = false) noexcept
                {
                    if(bForceDirection || is_directed(directed_edge))
                    {
                        this->node_1() = (node_index_t)index_1;
                        this->node_2() = (node_index_t)index_2;
                    }
                    else // direction ignored
                    {
                        this->node_1() = (node_index_t) tpf::minimum(index_1, index_2);
                        this->node_2() = (node_index_t) tpf::maximum(index_1, index_2);
                    }

                    this->weight() = (weight_t)weight;
                    return *this;
                }

                static int weak_compare_edges(const edge& lhs, const edge& rhs)
                {
                    if(lhs.node_1() == rhs.node_1())
                    {
                        if(lhs.node_2() == rhs.node_2())
                            return 0;
                        else if(lhs.node_2() < rhs.node_2())
                            return -1;
                        else // this->node_2() > rhs.node_2()
                            return 1;
                    }
                    else if(lhs.node_1() < rhs.node_1() )
                        return -1;
                    else // this->node_1() > rhs.node_1()
                        return 1;
                }

                static int strong_compare_edges(const edge& lhs, const edge& rhs)
                {
                    auto weak_compare = weak_compare_edges(lhs, rhs);

                    if(weak_compare!=0)
                        return weak_compare;
                    else
                    {
                        auto diff = lhs.weight() - rhs.weight();
                        return (diff == 0 ? 0: (diff < 0 ? -1: 1));   
                    }
                }
                
                struct weak_compare
                {
                    bool operator()(const edge& lhs, const edge& rhs) const
                    {
                        return weak_compare_edges(lhs, rhs) < 0;
                    }
                };

                struct strong_compare
                {
                    bool operator()(const edge& lhs, const edge& rhs) const
                    {
                        return strong_compare_edges(lhs, rhs) < 0;
                    }
                };

                inline static constexpr weak_compare weak_comparator{};
                inline static constexpr strong_compare strong_comparator{};
              
                template<typename IndexType>
                node_index_t other_node(IndexType node_index) const noexcept
                {
                    if(this->node_1() == (node_index_t)node_index)
                        return this->node_2();
                    else if(this->node_2() == (node_index_t)node_index)
                        return this->node_1();
                    else
                        return invalid_node_index;
                }
                
                friend bool operator < (const edge& lhs, const edge& rhs)
                {
                    return strong_compare_edges(lhs, rhs) < 0;
                }

                friend bool operator > (const edge& lhs, const edge& rhs)
                {
                    return strong_compare_edges(lhs, rhs) > 0;
                } 

                // partial order
                friend bool operator == (const edge& lhs, const edge& rhs)
                {
                    return strong_compare_edges(lhs, rhs) == 0;
                }

                friend bool operator != (const edge& lhs, const edge& rhs)
                {
                    return strong_compare_edges(lhs, rhs) != 0;
                }

                void swap(edge& rhs) noexcept
                {
                    if(this != std::addressof(rhs))
                        this->m_edge.swap(rhs.m_edge);
                }
                
                friend void swap(edge& lhs, edge& rhs) noexcept
                {
                    lhs.swap(rhs);
                }

                edge(const edge&) = default;
                edge& operator = (const edge&) = default;

                edge_tuple_t& get() noexcept { return this->m_edge; }
                const edge_tuple_t& get() const noexcept { return this->m_edge; }

                friend tpf::sstream& operator<<(tpf::sstream& os, const edge& e)
                {
                    os << "( "<<e.node_1()<< ", "<<e.node_2() << " | "<<e.weight() <<" )";
                    return os;
                }
        };

        template<int EdgeDirected, typename EdgeIndexType, typename... EdgeTypes, 
            template<typename, typename...> class ContainerType, typename... Types>
            tpf::sstream& operator << 
            (tpf::sstream& os, const ContainerType<edge<EdgeDirected, EdgeIndexType, EdgeTypes...>, Types...>& edges)
        {
            if(edges.empty())
            {
                os << "{ }"; return os;
            }
            else
            {
                auto last_edge = edges.cend();
                std::advance(last_edge, -1);

                for(auto itr = edges.cbegin(); itr != last_edge; ++itr)
                    os << *itr <<", ";

                os << *last_edge;
                
                return os;
            }
        }

        template<typename EdgeType, typename IndexType_1, typename IndexType_2, typename WeightType>
        EdgeType make_edge(IndexType_1 index_1, IndexType_2 index_2, WeightType weight, bool bForceDirection = false)
        {
            return EdgeType{ index_1, index_2, weight, bForceDirection};
        }

        template<typename EdgeType, typename IndexType_1, typename IndexType_2>
        EdgeType make_edge(IndexType_1 index_1, IndexType_2 index_2, bool bForceDirection = false)
        {
            return EdgeType{ index_1, index_2, bForceDirection};
        }

        template<int DirectedEdge, int EdgePlurality,
            typename ElementType, typename NodeIndexType, typename EdgeWeightType, 
            template<typename, typename...> class NodeContainerType,
            template<typename, typename...> class IndexContainerType,
            template<typename, typename...> class EdgeContainerType>
        class graph
        {
            template<int, typename, typename...> friend class edge;
            
            public:
                using element_t = ElementType;
                using index_t = NodeIndexType;

                using indices_t = std::vector<index_t>;
                using node_names_t = std::vector<ElementType>; 
                
                using node_index_t = NodeIndexType; 
                using edge_weight_t = EdgeWeightType;
                
                static constexpr int directed_edge = DirectedEdge;
                static constexpr int edge_multiplicity = EdgePlurality;

                using edge_type_t = edge<directed_edge, node_index_t, edge_weight_t>;
                
                using edge_container_t = EdgeContainerType<edge_type_t>;

                using index_container_t = IndexContainerType<index_t>; // container type for node indices 
                using node_container_t = NodeContainerType<index_container_t>; // container type for nodes
                using element_container_t = NodeContainerType<element_t>; // container type for node values
                
                using ref_element_container_t = NodeContainerType<std::reference_wrapper<element_t>>;
                using const_ref_element_container_t = NodeContainerType<std::reference_wrapper<const element_t>>;

                using node_ref_t = std::tuple<ElementType&, index_container_t&>;
                using const_node_ref_t = std::tuple<const ElementType&, const index_container_t&>;

                using node_info_t = std::tuple<const ElementType&, const_ref_element_container_t>;

                using distances_t = std::vector<edge_weight_t>;
                
                enum path_info: size_t { path_visited, path_distances, path_previous, path_shortest};

                using shortest_path_t = std::tuple<indices_t, distances_t, indices_t, edge_weight_t>;

                enum class visit_mode: int { graph, visit_bfs, visit_dfs};           

            private:
                
                node_container_t m_nodes; // container for node list
                element_container_t m_values; // container for each node's value
                edge_container_t m_edges; // container for edges


            public:

                const element_container_t& get_node_values() const
                {
                    return this->m_values;
                }

                decltype(auto) get_node_indices(index_t node_index) const
                {
                    return tpf::get_element<const node_container_t&>(this->m_nodes, node_index);
                }

                decltype(auto) get_node_indices(index_t node_index)
                {
                    return tpf::get_element<node_container_t&>(this->m_nodes, node_index);
                }

                decltype(auto) get_edge(index_t index)
                {
                    return tpf::get_element<edge_container_t&>(this->m_edges, (size_t)index);
                }

                decltype(auto) get_edge(index_t index) const
                {
                    return tpf::get_element<const edge_container_t&>(this->m_edges, (size_t)index);
                }

                indices_t get_edge_indices(index_t node_index) const
                {
                    indices_t edge_indices;

                    auto node_indices = this->get_node_indices(node_index);

                    tpf::sstream stream;
                    auto endl = tpf::endl;

                    for(auto& end_node: node_indices)
                    {
                        auto e = make_edge<edge_type_t>(node_index, end_node, edge_weight_t{});

                        auto index = tpf::binary_find_index(this->m_edges, e, edge_type_t::weak_comparator);

                        if(index < this->m_edges.size())
                        {
                            edge_indices.emplace_back((index_t)index);
                        }
                    }

                    return edge_indices;
                }

                indices_t get_edge_indices(index_t node_index, const indices_t& visited) const
                {
                    indices_t edge_indices;

                    auto node_indices = this->get_node_indices(node_index);

                    for(auto& end_node: node_indices)
                    {
                        if(set::is_in_container((index_t)end_node, visited)) continue;

                        auto e = make_edge<edge_type_t>(node_index, end_node, edge_weight_t{});

                        auto index = tpf::binary_find_index(this->m_edges, e, edge_type_t::weak_comparator);

                        if(index < this->m_edges.size())
                        {
                            edge_indices.emplace_back((index_t)index);
                        }
                    }

                    return edge_indices;
                }

                auto begin() { return this->m_nodes.begin(); }
                auto end() { return this->m_nodes.end(); }

                auto cbegin() { return this->m_nodes.cbegin(); }
                auto cend() { return this->m_nodes.cend(); }

                bool empty() const { return this->m_nodes.empty(); }
                
                decltype(auto) adjacency_node_list(size_t node_index)
                {
                    return tpf::get_element<node_container_t&>(m_nodes, node_index);
                }

                decltype(auto) adjacency_node_list(size_t node_index) const
                {
                    return tpf::get_element<const node_container_t&>(m_nodes, node_index);
                }

                const node_container_t& node_lists() const { return this->m_nodes; }
                const element_container_t& values() const { return this->m_values; }

                auto adjacent_node_values(size_t node_index)
                {
                    ref_element_container_t elements;
                    auto indices = tpf::get_element<node_container_t&>(m_nodes, node_index);

                    for(auto& idx: indices)
                    {
                        auto& value = tpf::get_element<element_container_t&>(m_values, idx);
                        elements.emplace_back(std::ref(value));
                    }

                    return elements;
                }

                auto adjacent_node_values(size_t node_index) const
                {
                    const_ref_element_container_t elements;
                    auto indices = tpf::get_element<const node_container_t&>(m_nodes, node_index);

                    for(auto& idx: indices)
                    {
                        auto& value = tpf::get_element<const element_container_t&>(m_values, idx);
                        elements.emplace_back(std::cref(value));
                    }

                    return elements;
                }
                
                decltype(auto) node_value(size_t node_index)
                {
                    return tpf::get_element<element_container_t&>(m_values, node_index);
                }

                decltype(auto) node_value(size_t node_index) const
                {
                    return tpf::get_element<const element_container_t&>(m_values, node_index);
                }
                
                template<typename IndexType_1, typename IndexType_2>
                auto get_edge_range_iterators(IndexType_1 index_1, IndexType_2 index_2) const
                {
                    auto e = make_edge<edge_type_t>(index_1, index_2, edge_weight_t{});
                    return tpf::find_range_iterators(this->m_edges, e, edge_type_t::weak_comparator);
                }

                template<typename IndexType_1, typename IndexType_2, typename WeightType>
                auto get_edge_range_iterators(IndexType_1 index_1, IndexType_2 index_2, WeightType weight) const
                {
                    auto e = make_edge<edge_type_t>(index_1, index_2, weight);
                    return tpf::find_range_iterators(this->m_edges, e, edge_type_t::strong_comparator);
                }

                auto get_edge_range_iterators(const edge_type_t& e, bool bStrongCompare = false) const
                {
                    if(bStrongCompare)
                        return tpf::find_range_iterators(this->m_edges, e, edge_type_t::strong_comparator);
                    else
                        return tpf::find_range_iterators(this->m_edges, e, edge_type_t::weak_comparator);
                }

                node_info_t node_info(size_t node_index) const 
                {
                    const_ref_element_container_t elements;
                    auto indices = tpf::get_element<const node_container_t&>(m_nodes, node_index);

                    for(auto& idx: indices)
                    {
                        auto& value = tpf::get_element<const element_container_t&>(m_values, idx);
                        elements.emplace_back(std::cref(value));
                    }

                    return { node_value(node_index), std::move(elements) };
                }
                
                tpf::sstream& get_node_name(tpf::sstream& os, size_t index) const
                {
                    os << "node_" << index; return os;
                }

                tpf::sstream& get_edge_definition(tpf::sstream& os, 
                    node_index_t index_1, node_index_t index_2,
                    bool bProportional = false, const char* color="black") const
                {
                    auto e = make_edge<edge_type_t>(index_1, index_2, edge_weight_t{});

                    auto [success, iterator] = 
                        tpf::binary_find_bool_iterator_pair(this->m_edges, e, edge_type_t::weak_comparator);

                    if(success)
                    {
                        os << " [ dir = none, color = \""<< color << "\", label = \" "
                            << iterator->weight() << " \"] ;";
                    }
                    
                    return os;    
                }
                
                tpf::sstream&  get_node_definition(tpf::sstream& os, size_t index) const
                {
                    auto& value = this->node_value(index);
                        
                    get_node_name(os, index) << " [ label =\" "
                        << value << " \"];";

                    return os;
                }

                tpf::sstream& build_graph_edges(tpf::sstream& os, 
                    visit_mode vmode = visit_mode::graph, bool bProportional = false) const
                {                
                    size_t size = this->m_nodes.size();
                    
                    for(size_t i=0; i <size; ++i)
                    {
                        get_node_definition(os, i) << "\n";
                    }

                    os << "\n";

                    using set_t = std::vector<index_t>;
                    using sets_t = std::vector<set_t>;

                    sets_t sets;

                    for(size_t i = 0; i < size; ++i)
                    {
                        auto nodes = this->adjacency_node_list(i);
                        for(const auto& v: nodes)
                        {
                            set_t s{(index_t)i}; 
                            s.emplace_back(v);
                            sets.push_back(s);
                        }
                    }

                    tpf::set::sort_in_place(sets);

                    for(auto& s: sets)
                    {
                        get_node_name(os, s[0]);

                        if(vmode == visit_mode::graph)
                            os << " -- ";
                        else
                            os << " -> ";
                        
                        get_node_name(os, s[1]);

                        if(vmode != visit_mode::graph)
                        {
                            get_edge_definition(os, (node_index_t)s[0], (node_index_t)s[1]);
                            os << "\n";
                        }
                        else
                            os << " ; \n";
                    }

                    if(vmode != visit_mode::graph)
                    {
                        std::vector<index_t> visit_order;

                        if(vmode == visit_mode::visit_bfs)
                            visit_order = traverse_bfs();
                        else 
                            visit_order = traverse_dfs();
                    
                        if(visit_order.size() > 1)
                        {
                            os << "\n";
                            
                            for(size_t i = 1; i < visit_order.size(); ++i )
                            {
                                get_node_name(os, (size_t)visit_order[i-1]);
                                os << " -> ";
                                get_node_name(os, (size_t)visit_order[i]);

                                os << " [ color = blue ] ; \n";
                            }
                        }
                    }
                    return os;
                }

                std::string draw_graph_traversal(visit_mode vmode = visit_mode::visit_dfs) const
                {
                    tpf::sstream os;

                    os << "digraph G {\n";

                    if(vmode == visit_mode::visit_bfs)
                        os << "label=\"Breadth First Search\"\n";
                    else
                        os << "label=\"Depth First Search\"\n";

                    build_graph_edges(os, vmode);
                    
                    os << "}";

                    return os.str();

                }

                tpf::sstream& draw_graph_edges(tpf::sstream& os) const
                {                
                    size_t size = this->m_nodes.size();
                    
                    for(size_t i=0; i <size; ++i)
                    {
                        get_node_definition(os, i) << "\n";
                    }

                    os << "\n";
                   
                    for(const auto& edge: this->m_edges)
                    {
                        get_node_name(os, edge.node_1());
                        os << " -> ";
                        get_node_name(os, edge.node_2());

                        os << " [ "; 
                        
                        if(!is_directed(directed_edge))
                        os << "dir = none, ";

                        os << "label = \" " << edge.weight() << " \"];";

                        os << "\n";
                        
                    }
                    
                    return os;
                }

                std::string draw_graph(
                    const std::string& graph_title = "Plain Graph",
                    const std::string& graph_style = "", 
                    const std::string& best_viewed = "dot") const
                {
                    tpf::sstream os;

                    os << "digraph G {\n\n";
                    
                    if(graph_style != "")
                        os << graph_style << "\n";
                    
                    os << "label=\"";
                    os << graph_title <<"\\nBest Viewed: ";
                    os << best_viewed << "\";\n\n";
                    
                    draw_graph_edges(os);
                    
                    os << "}";

                    return os.str();

                }               

                std::string build_graph(visit_mode vmode = visit_mode::graph) const
                {
                    if(vmode == visit_mode::graph)
                    {
                        tpf::sstream os;

                        os << "digraph G {\n";
                        os << "label=\"Plain Graph\"\n";

                        build_graph_edges(os);
                        
                        os << "}";

                        return os.str();
                    }
                    else
                    {
                        tpf::sstream os;

                        os << "digraph G {\n";

                        if(vmode == visit_mode::visit_bfs)
                            os << "label=\"Breadth First Search\"\n";
                        else
                            os << "label=\"Depth First Search\"\n";

                        build_graph_edges(os, vmode);
                        
                        os << "}";

                        return os.str();
                    }
                }

                graph() = default;

                size_t size() { return this->m_nodes.size(); }

                // throws exception if fails
                void integrity_check_nodes_throws() const
                {
                    size_t size = this->m_nodes.size();
                    for(const auto& node: this->m_nodes)
                    {
                        for(const auto& e: node)
                        {
                            if(e >= size)
                            {
                                tpf::sstream msg;
                                msg << "node " << node << " has an adjacent vertex " << e
                                    <<" (max node index " << (size - 1)
                                    << ") which does not exist in the node list ";
                                
                                Tpf_ThrowDebugException(msg);
                            }
                        }
                    }
                }

                template<typename EleType, typename Type, typename... Types>
                void emplace_back_node(EleType&& value, Type&& index0, Types&&... indices)
                {
                    auto v = tpf::make_random_access_container<IndexContainerType>((NodeIndexType)std::forward<Type>(index0), 
                                                                (NodeIndexType)std::forward<Types>(indices)...);
                    tpf::set::sort_in_place(v);
                    
                    this->m_nodes.emplace_back(std::move(v));
                    this->m_values.emplace_back(std::forward<EleType>(value));
                }

                auto is_in_edge_list(const edge_type_t& e, bool bStrongCompare = false) const noexcept
                {
                    if(bStrongCompare)
                        return tpf::binary_find_bool_iterator_pair(this->m_edges, e, edge_type_t::strong_comparator);
                    else
                        return tpf::binary_find_bool_iterator_pair(this->m_edges, e, edge_type_t::weak_comparator);
                }

                edge_container_t& edges() noexcept { return this->m_edges; }
                const edge_container_t& edges() const noexcept { return this->m_edges; }


                template<typename IndexType_1, typename IndexType_2, typename... WeightType>
                void emplace_back_edge(IndexType_1 index_1, IndexType_2 index_2, WeightType... weight)
                {
                    size_t size = this->m_nodes.size();

                    if( (size_t)index_1 >= size)
                    {
                        tpf::sstream msg;

                        msg <<"The edge (" << index_1 << ", " << index_2
                            <<") is not VALID, its first vertex index "<< index_1
                            << " is out of range (max node index " << (size-1) <<").";

                        Tpf_ThrowDebugException(msg);
                    }

                    if( (size_t)index_2 >= size)
                    {
                        tpf::sstream msg;

                        msg <<"The edge (" << index_1 << ", " << index_2
                            <<") is not VALID, its second vertex index "<< index_2
                            << " is out of range (max node index " << (size-1) <<").";

                        Tpf_ThrowDebugException(msg);
                    }

                    if constexpr(is_weighted(directed_edge))
                    {
                        static_assert(sizeof...(WeightType) == 1, "Weight of edge should be specified");
                       
                        auto e = make_edge<edge_type_t>(index_1, index_2, weight...);

                        if constexpr(edge_multiplicity == edge_plurality::single_edged)
                        {
                            // false for "weak compare, ignore weight."
                            auto [already_exists, iterator] = is_in_edge_list(e, false); 
                            if(already_exists)
                            {
                                // strong comparison
                                if(e == *iterator) return; // ignore

                                tpf::sstream msg;
                                msg << "new edge " << e << " is different from the existing edge "
                                    << *iterator <<".\nMultiple different edges incident on the same node pairs are not allowed in a signle-edged graph.";

                                Tpf_ThrowDebugException(msg);
                            }
                        }
                        else
                        {
                            // true for "strong compare, respect weight."
                            auto [already_exists, iterator] = is_in_edge_list(e, true); 
                            if(already_exists) return; // ignore exact match
                            // {
                            //     tpf::sstream msg;
                            //     msg << "new edge " << e << " is different from the existing edge "
                            //         << *iterator <<". Multiple different edges incident on the same node pairs are not allowed";

                            //     Tpf_ThrowDebugException(msg);
                            // }
                        }
                        
                        this->m_edges.push_back(std::move(e));
                        set::sort_in_place(this->m_edges);
                    }
                    else
                    {
                        static_assert(sizeof...(WeightType) == 0, "Weight of edge is not allowed");

                        auto e = make_edge<edge_type_t>(index_1, index_2);
                        
                        auto [already_exists, iterator] = is_in_edge_list(e);
                        if(already_exists) return;

                        this->m_edges.push_back(std::move(e));
                        set::sort_in_place(this->m_edges);
                    }
                }
            
                auto operator[](size_t node_index)
                {
                    return node_ref_t{ tpf::get_element<element_container_t&>(this->m_values, node_index),
                        tpf::get_element<node_container_t&>(this->m_nodes, node_index) };
                }

                auto operator[](size_t node_index) const
                {
                    return const_node_ref_t{ tpf::get_element<const element_container_t&>(this->m_values, node_index),
                        tpf::get_element<const node_container_t&>(this->m_nodes, node_index) };
                }              

                std::vector<index_t> traverse_bfs(index_t start = 0) const
                {
                    tpf::sstream stream;
                    auto endl = tpf::endl;
                    
                    std::deque<index_t> queue;
                    std::vector<char> visited(m_nodes.size());
                    std::vector<index_t> traversal;
                    
                    for(auto& v: visited) v = 0;
                    queue.push_back(start);

                    visited[(size_t)start] = 1;

                    while(!queue.empty())
                    {
                        auto p = queue.front();
                        queue.pop_front();

                        traversal.push_back(p);
                        
                        auto node_list = this->adjacency_node_list((size_t)p); 
                        for(auto& node: node_list)
                        {
                            if(visited[(size_t)node] == 0)
                            {
                                queue.push_back(node);
                                visited[(size_t)node] = 1;

                            }
                        }                  
                    }

                    return traversal;
                }

                void push_stack(std::deque<index_t>& stack,
                    std::vector<char>& visited,
                    size_t node_index) const
                    {
                        const index_container_t& node_list
                            = this->adjacency_node_list(node_index);

                        for(const auto& node: node_list)
                        {
                            if(visited[(size_t)node] != 1)
                            {
                                stack.push_front(node);
                                visited[(size_t)node] = 1;
                            }
                        } 
                    }
                
                void move_vertex(indices_t& visited, indices_t& unvisited, index_t index) const
                {
                    visited.push_back(index);

                    auto itr = std::find(unvisited.cbegin(), unvisited.cend(), index);

                    if(itr != unvisited.end()) unvisited.erase(itr);
                }

                element_t translate(index_t index) const
                {
                    size_t size = this->m_values.size();

                    if((size_t)index < this->m_values.size())
                        return tpf::get_element(this->m_values, (size_t)index);
                    else
                        return "Unvisited";
                }

                node_names_t translate(const indices_t& indices) const
                {
                    size_t size = indices.size();
                    node_names_t names; names.reserve(size);

                    for(const auto& index: indices)
                    {
                        if((size_t)index < this->m_values.size())
                        {  auto e = tpf::get_element(this->m_values, (size_t)index);
                            names.emplace_back(e);
                        }
                        else
                        {
                            names.emplace_back("Unvisited");
                        }
                        
                    }

                    return names;
                }

                shortest_path_t shortest_path(index_t start_node = 0, index_t end_node=0) const
                {                
                    tpf::sstream stream;
                    auto endl = tpf::endl;

                    size_t size = this->m_nodes.size();

                    indices_t unvisited(size);
                    std::iota(unvisited.begin(), unvisited.end(), 0);
                    
                    indices_t visited; visited.reserve(size);

                    indices_t previous_vertices;
                    distances_t shortest_distances;

                    auto max_weight = tpf::type_max_v<edge_weight_t>;
                    auto max_index = tpf::type_max_v<index_t>;

                    for(size_t i=0; i < size; ++i)
                    {
                        if(i != (size_t)start_node)
                        {
                            shortest_distances.emplace_back(max_weight);
                            previous_vertices.emplace_back(max_index);
                        }
                        else
                        {
                            shortest_distances.emplace_back(edge_weight_t{});
                            previous_vertices.emplace_back(start_node);
                        }
                    }

                    auto edge_indices = get_edge_indices(start_node);

                    for(auto edge_index: edge_indices)
                    {
                        auto edge = get_edge(edge_index);
                        auto other_node = edge.other_node((node_index_t)start_node);

                        shortest_distances[(size_t)other_node] = edge.weight();

                        previous_vertices[(size_t)other_node] = (index_t)start_node;
                    }

                    this->move_vertex(visited, unvisited, start_node);

                    while(!unvisited.empty())
                    {
                        start_node = (index_t)set::minimum_value_index(shortest_distances, visited);
                        edge_weight_t start_weight = shortest_distances[(size_t)start_node];

                        edge_indices = this->get_edge_indices(start_node, visited);

                        for(auto edge_index: edge_indices)
                        {
                            auto edge = get_edge(edge_index);
                            auto other_node = edge.other_node((node_index_t)start_node);

                            auto new_weight = start_weight + edge.weight();

                            if( new_weight < shortest_distances[(size_t)other_node])
                            {
                                shortest_distances[(size_t)other_node] = new_weight;
                                previous_vertices[(size_t)other_node] = (index_t)start_node;
                            }
                        }
                        
                        this->move_vertex(visited, unvisited, start_node);
                    }

                    return {visited, shortest_distances, 
                        previous_vertices, shortest_distances[(size_t)visited.back()]};
                }

                tpf::sstream& draw_edge_list(tpf::sstream& os,
                    const indices_t& visited, const distances_t& distances,
                    const indices_t& previous_node, index_t start_node, index_t end_node) const
                {   
                    std::deque<edge_type_t> itinerary;

                    while(start_node != end_node)
                    {
                        auto distance = distances[(size_t)end_node];

                        auto prev_idx = previous_node[end_node];

                        auto e = make_edge<edge_type_t>(prev_idx, end_node, distance, true);

                        itinerary.push_front(e);

                        end_node = prev_idx;
                    }
                    
                    for(const auto& edge: itinerary)
                    {
                        get_node_name(os, edge.node_1());
                        os << " -> ";
                        get_node_name(os, edge.node_2());

                        os << " [ style = dashed, color= blue, fontcolor = blue, ";

                        os << "label = \" " << edge.weight() << " \"];";

                        os << "\n";
                        
                    }
                    
                    return os;
                }

                void draw_shortest_path(tpf::sstream& os,
                    const indices_t& visited, const distances_t& distances,
                    const indices_t& previous_node, index_t start_node, index_t end_node) const
                {
                    const std::string& graph_title = "Shortest Path";
                    const std::string& graph_style = "pack=100;\npackmode=array;\nedge [len=2];";
                    const std::string& best_viewed = "neato or osage";

                    os << "digraph G {\n\n";
                    
                    if(graph_style != "")
                        os << graph_style << "\n";
                    
                    os << "label=\"";
                    os << graph_title <<"\\nBest Viewed: ";
                    os << best_viewed << "\";\n\n";
                    
                    draw_graph_edges(os);
                    
                    os << "\n";

                    draw_edge_list(os, visited, distances, previous_node, start_node, end_node);

                    os << "\n";
                    
                    os << "}";
                }

                std::string shortest_path_report(element_t start_pos, element_t end_pos) const
                {                    
                    tpf::sstream stream;
                    
                    auto endl = "\n";
                    auto endlL = "\n\n";

                    auto start_itr  = std::find(this->m_values.cbegin(), this->m_values.cend(), start_pos);
                    auto end_itr = std::find(this->m_values.cbegin(), this->m_values.cend(), end_pos);

                    if(start_itr == this->m_values.cend())
                    {
                        stream << start_pos << " is not in the node list";
                        Tpf_ThrowDebugException(stream);
                    }

                    if(end_itr == this->m_values.cend())
                    {
                        stream << end_pos << " is not in the node list";
                        Tpf_ThrowDebugException(stream);
                    }

                    index_t start_node = std::distance(this->m_values.cbegin(), start_itr);
                    index_t end_node = std::distance(this->m_values.cbegin(), end_itr);

                    stream <<"Start: " << start_node << endl;
                    stream <<"Start: " << end_node << endl;
                    
                    auto [route, distances, previous_node, shortest] = 
                        this->shortest_path(start_node, end_node);

                    stream <<"Shortest Path: " << this->translate(route) << endL;
                    stream <<"Distances:     " << this->get_node_values() << endl;
                    stream <<"from "<< this->translate(start_node) <<"       : " << distances << endl;
                    stream <<"Previous node: " << this->translate(previous_node) << endL;

                    stream <<"Shortest distance from " << this->translate(start_node) <<" to " 
                        << this->translate(end_node) << " is " << distances[(size_t)end_node]; 

                    stream << "\n\n";

                    draw_shortest_path(stream, route, distances, previous_node, start_node, end_node);

                    return stream.str();
                }

                std::vector<index_t> traverse_dfs(index_t start=0) const
                {
                    tpf::sstream stream;
                    auto endl = tpf::endl;
                    
                    std::deque<index_t> stack;
                    std::vector<char> visited(this->m_nodes.size());
                    std::vector<index_t> traversal;
                    
                    for(auto& v: visited) v = 0;
                    
                    push_stack(stack, visited, (size_t)start);

                    while(!stack.empty())
                    {
                        traversal.push_back(stack.front());
                        stack.pop_front();
                        push_stack(stack, visited, (size_t)traversal.back());
                    }

                    return traversal;
                }

                    
                friend tpf::sstream& operator << (tpf::sstream& os, const graph& g)
                {
                    if(g.m_nodes.empty())
                    {
                        os << "--------\n";
                        os << " empty \n";
                        os << "--------\n";
                        
                        return os;
                    }
                    
                    auto last_itr = g.m_nodes.cend();
                    
                    os << "--------\n";

                    using element_container_t = typename graph::element_container_t;

                    size_t index = 0;
                    
                    for(auto itr = g.m_nodes.cbegin(); itr != g.m_nodes.cend(); ++itr)
                        os << std::setw(3) << index << " - (" 
                            << tpf::get_element<const element_container_t&>(g.m_values, index++)<<") " << *itr << "\n";

                    os << "--------\n";

                    os << g.m_edges << "\n";

                    os << "--------";

                    return os;
                }
        };

        template<int DirectedEdge, int EdgePlurality = edge_plurality::single_edged,
            typename ElementType = const char*, typename NodeIndexType = int,
            typename EdgeWeightType = float, 
            template<typename, typename...> class NodeContainerType = std::list,
            template<typename, typename...> class IndexContainerType = std::vector,
            template<typename, typename...> class EdgeContainerType = std::vector>
        using graph_t = graph<DirectedEdge, EdgePlurality, ElementType, NodeIndexType, EdgeWeightType,
                    NodeContainerType, IndexContainerType, EdgeContainerType>;
    
    } // end of namespace graph

} // end of namespace tpf

#ifdef _TPF_GRAPH_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_GRAPH_MIN_DEFINED
#endif 

#ifdef _TPF_GRAPH_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_GRAPH_MAX_DEFINED
#endif 

#endif // end of file _TPF_GRAPH_HPP

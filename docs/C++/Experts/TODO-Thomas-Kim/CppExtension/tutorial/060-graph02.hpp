#include <tpf_set.hpp>

namespace  tpf 
{
    template<typename ElementType, typename NodeIndexType, 
        template<typename, typename...> class NodeContainerType,
        template<typename, typename...> class IndexContainerType>
    class graph
    {
        public:
            using element_t = ElementType;
            using index_t = NodeIndexType;
            using index_container_t = IndexContainerType<index_t>; // container type for node indices 
            using node_container_t = NodeContainerType<index_container_t>; // container type for nodes
            using element_container_t = NodeContainerType<element_t>; // container type for node values
            
            using ref_element_container_t = NodeContainerType<std::reference_wrapper<element_t>>;
            using const_ref_element_container_t = NodeContainerType<std::reference_wrapper<const element_t>>;

            using node_ref_t = std::tuple<ElementType&, index_container_t&>;
            using const_node_ref_t = std::tuple<const ElementType&, const index_container_t&>;

            using node_info_t = std::tuple<const ElementType&, const_ref_element_container_t>;

            enum class visit_mode: int { graph, visit_bfs, visit_dfs };
            
        private:
            
            node_container_t m_nodes; // container for node list
            element_container_t m_values; // container for each node's value
 
        public:

            auto begin() { return this->m_nodes.begin(); }
            auto end() { return this->m_nodes.end(); }

            auto cbegin() { return this->m_nodes.cbegin(); }
            auto cend() { return this->m_nodes.cend(); }

            bool empty() const { return this->m_nodes.empty(); }
            
            decltype(auto) adjacency_node_list(size_t node_index)
            {
                return tpf::element<node_container_t&>(m_nodes, node_index);
            }

            decltype(auto) adjacency_node_list(size_t node_index) const
            {
                return tpf::element<const node_container_t&>(m_nodes, node_index);
            }

            const node_container_t& node_lists() const { return this->m_nodes; }
            const element_container_t& values() const { return this->m_values; }

            auto adjacent_node_values(size_t node_index)
            {
                ref_element_container_t elements;
                auto indices = tpf::element<node_container_t&>(m_nodes, node_index);

                for(auto& idx: indices)
                {
                    auto& value = tpf::element<element_container_t&>(m_values, idx);
                    elements.emplace_back(std::ref(value));
                }

                return elements;
            }

            auto adjacent_node_values(size_t node_index) const
            {
                const_ref_element_container_t elements;
                auto indices = tpf::element<const node_container_t&>(m_nodes, node_index);

                for(auto& idx: indices)
                {
                    auto& value = tpf::element<const element_container_t&>(m_values, idx);
                    elements.emplace_back(std::cref(value));
                }

                return elements;
            }
            
            decltype(auto) node_value(size_t node_index)
            {
                return tpf::element<element_container_t&>(m_values, node_index);
            }

            decltype(auto) node_value(size_t node_index) const
            {
                return tpf::element<const element_container_t&>(m_values, node_index);
            }

            node_info_t node_info(size_t node_index) const 
            {
                const_ref_element_container_t elements;
                auto indices = tpf::element<const node_container_t&>(m_nodes, node_index);

                for(auto& idx: indices)
                {
                    auto& value = tpf::element<const element_container_t&>(m_values, idx);
                    elements.emplace_back(std::cref(value));
                }

                return { node_value(node_index), std::move(elements) };
            }
            
            tpf::sstream& get_node_name(tpf::sstream& os, size_t index)
            {
                os << "node_"<<index; return os;
            }

            tpf::sstream&  get_node_definition(tpf::sstream& os, size_t index)
            {
                auto& value = this->node_value(index);
                    
                get_node_name(os, index) << " [ label =\""
                    << value << "\"];";

                return os;
            }

            tpf::sstream& build_graph_edges(tpf::sstream& os, visit_mode vmode = visit_mode::graph)
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
                        set_t s{(index_t)i}; s.emplace_back(v);
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
                        os << " [dir=none, color=red, style=dashed] ; \n";
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
            
            std::string build_graph(visit_mode vmode = visit_mode::graph)
            {
                if(vmode == visit_mode::graph)
                {
                    tpf::sstream os;

                    os << "graph G {\n";
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

            template<typename EleType, typename Type, typename... Types>
            void emplace_back(EleType&& value, Type&& index0, Types&&... indices)
            {
                auto v = tpf::make_random_access_container<IndexContainerType>((NodeIndexType)std::forward<Type>(index0), 
                                                            (NodeIndexType)std::forward<Types>(indices)...);
                tpf::set::sort_in_place(v);
                
                this->m_nodes.emplace_back(std::move(v));
                this->m_values.emplace_back(std::forward<EleType>(value));
            }
            
            auto operator[](size_t node_index)
            {
                return node_ref_t{ tpf::element<element_container_t&>(this->m_values, node_index),
                    tpf::element<node_container_t&>(this->m_nodes, node_index) };
            }

            auto operator[](size_t node_index) const
            {
                return const_node_ref_t{ tpf::element<const element_container_t&>(this->m_values, node_index),
                    tpf::element<const node_container_t&>(this->m_nodes, node_index) };
            }              

            std::vector<index_t> traverse_bfs(index_t start = 0)
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
                size_t node_index)
                {
                    index_container_t& node_list
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
            
            std::vector<index_t> traverse_dfs(index_t start=0)
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
                        << tpf::element<const element_container_t&>(g.m_values, index++)<<") " << *itr << "\n";

                os << "--------";

                return os;
            }
    };

    template<typename ElementType = int, typename NodeIndexType = int, 
        template<typename, typename...> class NodeContainerType = std::list,
        template<typename, typename...> class IndexContainerType = std::vector>
    using graph_t = graph<ElementType, NodeIndexType, NodeContainerType, IndexContainerType>;

} // end of namespace tpf
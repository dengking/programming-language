#include <tpf_output.hpp>

tpf::sstream stream;

auto endl = tpf::endl; // single carriage return and flush to console output
auto endL = tpf::endL; // double carriage returns and flush to console output

auto nl = tpf::nl; // single carriage return without flush
auto nL = tpf::nL; // double carriage returns without flush

using string_t = std::string;

template<typename ElementType>
class binary_node
{
    public:
        using node_ptr_t = std::unique_ptr<binary_node>;
        enum class visit_mode: int{ pre_order, in_order, 
            ascending_order = in_order, post_order, descending_order };

    private:
        
        ElementType m_value; // node's value
        
        binary_node* m_parent; // pointer to point to the parent node 

        node_ptr_t m_left; // left child node
        node_ptr_t m_right; // right child node

    public:

        tpf::sstream& get_node_name(tpf::sstream& os)
        {
            os << "node_" << this->m_value;
            return os;
        }

        tpf::sstream& get_node_definition(tpf::sstream& os)
        {
            // node_1 [ shape=box, label = " Root "] ;
            get_node_name(os) 
                << "  [ shape=oval, label = \" "
                << this->m_value << " \"] ;";

            return os;
        }

        void print_node(tpf::sstream& os)
        {
            get_node_definition(os) << nl;

            // root -> left
            if(this->m_left)
            {
                get_node_name(os) << " -> ";
                this->m_left->get_node_name(os) << ";" << nl;

                // recursion for left children
                this->m_left->print_node(os);
            }

            // root -> right
            if(this->m_right)
            {
                get_node_name(os) << " -> ";
                this->m_right->get_node_name(os) << ";" << nl;
                
                // recursion for right children
                this->m_right->print_node(os);
            }
        }
        string_t build_digraph()
        {
            tpf::sstream os;

            os << "digraph G { " << nl;

            print_node(os);

            os <<"}";

            return os.str();
        }

        ElementType get() const { return this->m_value; }

        // if find() fails, returns nullptr,
        // if successful, it returns the pointer that points
        // to the node that has the value
        binary_node* find(ElementType value)
        {
            if(value == this->m_value)
                return this;
            
            binary_node* ptr = nullptr;

                                // recursion
            if(this->m_left && (ptr = this->m_left->find(value)))
                return ptr;
                                // recursion
            if(this->m_right && (ptr = this->m_right->find(value)))
                return ptr;
                
            return nullptr;
        }

        binary_node(ElementType value = ElementType{}, binary_node* parent = nullptr):
            m_value{value}, m_parent{parent} { }

        // if insertion fails,
        // returns false
        bool insert(ElementType value)
        {
            if(value == this->m_value)
                return false;
            else if(value < this->m_value)
            {
                if(this->m_left)
                    // this is recursion
                    return this->m_left->insert(value);
                else
                {
                    this->m_left.reset( new binary_node{value, this} );
                    return true;
                }
            }
            else // value > this->m_value
            {
                if(this->m_right)
                    // this is also recursion
                    return this->m_right->insert(value);
                else
                {
                    this->m_right.reset( new binary_node{value, this});
                    return true;
                }
            }
        }

        // return true only if all parameters are inserted
        // successfully, otherwise returns false
        template<typename Type, typename... Types>
        bool insert(Type arg, Types... args)
        {
            bool result = this->insert(arg);

            if constexpr(sizeof...(args) != 0)
            {
                                    // recursion
                return result && this->insert(args...);
            }
            else
                return result;
        }

        // for more information about visiting order
        // Binary Tree (ALL Interview Questions)
        // https://www.youtube.com/watch?v=VQTF_pRTZek&list=PLeIMaH7i8JDj7DnmO7lll97P1yZjMCpgY&index=2
        void visit_nodes(tpf::sstream& os, visit_mode order = visit_mode::in_order)
        {
            switch(order)
            {
                case visit_mode::pre_order:
                {
                    os << this->m_value << nl;

                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    if(this->m_right)
                        this->m_right->visit_nodes(os, order);

                    return;
                }
                case visit_mode::post_order:
                {
                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    if(this->m_right)
                        this->m_right->visit_nodes(os, order);

                    os << this->m_value << nl;

                    return;
                }

                case visit_mode::descending_order:
                {
                    if(this->m_right)
                        this->m_right->visit_nodes(os, order);

                    os << this->m_value << nl;

                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    return;
                }

                case visit_mode::ascending_order:
                default: // in_order
                {
                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    os << this->m_value << nl;
                    
                    if(this->m_right)
                        this->m_right->visit_nodes(os, order); 

                    return;
                }
            }
        }
};

void test_ascending_descending_order()
{
    // In courtesy of Vivekanand Khyade - Algorithm Every Day
    // Introduction to AVL tree (Why AVL tree is needed?)
    // https://youtu.be/5Q-__zhQ2Gs?t=10

    binary_node<int> root{10};

    using visit_mode = binary_node<int>::visit_mode;

    root.insert(8);
    root.insert(15);

    root.insert(7);
    root.insert(9);
    root.insert(12);
    root.insert(17);

    root.insert(6);
    root.insert(16);
    root.insert(18);

    tpf::sstream os;

    root.visit_nodes(os, visit_mode::ascending_order);
    stream << "Ascending-order: " << endL;
    stream << os.str() << endL;

    os.clear();

    root.visit_nodes(os, visit_mode::descending_order);
    stream << "Descending-order: " << endL;
    stream << os.str() << endL;
}

void test_find_binary_tree()
{
    // In courtesy of Vivekanand Khyade - Algorithm Every Day
    // Introduction to AVL tree (Why AVL tree is needed?)
    // https://youtu.be/5Q-__zhQ2Gs?t=10

    binary_node<int> root{10};

    using visit_mode = binary_node<int>::visit_mode;

    root.insert(8, 15, 7, 9, 12, 17, 6, 16, 18, 25);

    if(auto ptr = root.find(6))
    {
        stream << "Value found: " << ptr->get() << endl;
    } 
    else
    {
        stream <<"Value not found " << endl;
    }

    if(auto ptr = root.find(20) )
    {
        stream << "Value found: " <<  ptr->get() << endl;
    }
    else
    {
        stream << "Value not found " << endl;
    }
}


void test_build_digraph()
{
    // In courtesy of Vivekanand Khyade - Algorithm Every Day
    // Introduction to AVL tree (Why AVL tree is needed?)
    // https://youtu.be/5Q-__zhQ2Gs?t=10

    binary_node<int> root{10};

    using visit_mode = binary_node<int>::visit_mode;

    root.insert(8, 15, 7, 9, 12, 17, 6, 16, 18);

    stream << root.build_digraph() << endl;
    
}


int main(int argc, const char* argv[])
{
    // test_ascending_descending_order();
    
    // test_find_binary_tree();

    // test_build_digraph();

    if(argc < 2)
    {
        // Introduction to AVL tree (Why AVL tree is needed?)
	    //v https://youtu.be/5Q-__zhQ2Gs?t=29

        // bin_graph 10 8 15 7 9 12 17 6 16 18 > binary_tree.gv

        stream << "Usage: " << argv[0] << "  node list " << endl;
        stream <<"Example: "<<argv[0] <<" 10 8 15 7 9 12 17 6 16 18 > binary_tree.gv" << endl;
        stream << "dot -Tpng binary_tree.gv -o binary_tree.png" << endl;
        
        return 0;
    }

    binary_node<int> root { std::atoi(argv[1])};

    for(int i = 2; i < argc; ++i)
    {
        root.insert( std::atoi(argv[i]) );
    }
    
    stream << root.build_digraph() << endl;
}
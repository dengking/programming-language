#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl; // single carriage return and flush to console output
auto endL = tpf::endL; // double carriage returns and flush to console output

auto nl = "\n"; // single carriage return without flush
auto nL = "\n\n"; // double carriage returns without flush

template<typename ElementType>
class binary_node
{
    public:
        using node_ptr_t = std::unique_ptr<binary_node>;
        enum class visit_mode: int{ pre_order, in_order, ascending_order = in_order, post_order, descending_order };

    private:
        
        ElementType m_value; // node's value
        
        binary_node* m_parent; // pointer to point to the parent node 

        node_ptr_t m_left; // left child node
        node_ptr_t m_right; // right child node

    public:
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

        // for more information about visiting order
        // Binary Tree (ALL Interview Questions)
        // https://www.youtube.com/watch?v=VQTF_pRTZek&list=PLeIMaH7i8JDj7DnmO7lll97P1yZjMCpgY&index=2
        void visit_nodes(std::stringstream& os, visit_mode order = visit_mode::in_order)
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

void test_binary_tree()
{
    // In courtesy of Yoav Freund's AVL trees
    // https://youtu.be/YKt1kquKScY?t=402

    binary_node<int> root{50};

    using visit_mode = binary_node<int>::visit_mode;

    root.insert(17);
    root.insert(72);

    root.insert(12);
    root.insert(23);

    root.insert(54);
    root.insert(76);

    root.insert(9);
    root.insert(14);
    root.insert(19);
    root.insert(67);

    std::stringstream os;

    root.visit_nodes(os, visit_mode::pre_order);
    stream << "Pre-order: " << endL;
    stream << os.str() << endL;

    os.str("");
    root.visit_nodes(os, visit_mode::in_order);
    stream << "In-order: " << endL;
    stream << os.str() << endL;

    os.str("");
    root.visit_nodes(os, visit_mode::post_order);
    stream << "post-order: " << endL;
    stream << os.str() << endL;
}

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

    std::stringstream os;

    root.visit_nodes(os, visit_mode::ascending_order);
    stream << "Ascending-order: " << endL;
    stream << os.str() << endL;

    os.str("");
    root.visit_nodes(os, visit_mode::descending_order);
    stream << "Descending-order: " << endL;
    stream << os.str() << endL;
}

int main()
{
    // test_binary_tree();
    test_ascending_descending_order();
}
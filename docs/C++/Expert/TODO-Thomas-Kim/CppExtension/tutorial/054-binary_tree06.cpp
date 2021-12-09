#include <tpf_output.hpp>

/*
    binary_tree 50 17 72 12 23 54 76 9 14 19 67 > old.txt

    binary_tree 50 17 72 12 23 54 76 9 14 19 67 25 > new.txt

    binary_tree 50 17 72 12 23 54 76 9 14 19 67 25 18> newnew.txt

    https://edotor.net/

 */

tpf::sstream stream;

auto endl = tpf::endl; // single carriage return and flush to console output
auto endL = tpf::endL; // double carriage returns and flush to console output

auto nl = tpf::nl; // single carriage return without flush
auto nL = tpf::nL; // double carriage returns without flush

using string_t = std::string;

template<typename ReturnType, typename... Types>
using enable_if_all_types_are_the_same_t = 
    std::enable_if_t<tpf::types::is_same_v<tpf::remove_cv_ref_t<Types>...>, ReturnType>;

template<typename ElementType>
class binary_node
{
    public:
        using node_ptr_t = std::unique_ptr<binary_node>;
        enum class visit_mode: int{ undefined = 0, pre_order, in_order, 
            ascending_order = in_order, post_order, descending_order };

        enum class find_mode: int { undefined = 0, predecessor = 1, exact_match = 2, successor = 3 };

        enum class child_status: int { left_child = -1, no_child = 0, right_child = 1};
    private:
        
        ElementType m_value; // node's value
        
        mutable int m_height{1}; // the height of a node

        binary_node* m_parent; // pointer to point to the parent node 

        node_ptr_t m_left; // left child node
        node_ptr_t m_right; // right child node

    public:

        int height(bool bRecalculate = false) const
        {
            if(bRecalculate) // if bRecalculate is true,
            {                // we will recalculate the m_height member
                int left_height = 1, right_height = 1;

                if(this->m_left)
                    left_height += this->m_left->height(false); // false means we use cached value of m_height

                if(this->m_right)
                    right_height += this->m_right->height(false); // false means we use cached value of m_height

                // m_height is the greater of the two, left_height and right_height
                this->m_height = left_height > right_height ? 
                        left_height : right_height;
            }

            return this->m_height;
        }

        void update_height()
        {
            // the parent ptr of the current node
            auto parent_ptr = this->m_parent;
            
            while(parent_ptr)
            {
                auto old_height = parent_ptr->m_height;

                if(old_height != parent_ptr->height(true))
                    parent_ptr = parent_ptr->m_parent;
                else
                    break;
            }
        }

        child_status get_child_status(binary_node* child)
        {
            if(this->m_left && this->m_left.get() == child)
            
                return child_status::left_child;
            else if(this->m_right && this->m_right.get() == child)

                return child_status::right_child;
            else 
                return child_status::no_child;
        }

        tpf::sstream& get_node_name(tpf::sstream& os)
        {
            os << "node_" << this->m_value;
            return os;
        }

        tpf::sstream& get_node_definition(tpf::sstream& os)
        {
            // node_1 [ shape=box, label = " Root "] ;
            get_node_name(os) 
                << "  [ shape=oval, label = \"m_value: "
                << this->m_value 
                /* << " H:"<< this->m_height */ << " \"] ;";

            return os;
        }

        void print_node(tpf::sstream& os)
        {
            get_node_definition(os) << nl;

            if(this->m_parent)
            {
                get_node_name(os) << " -> ";
                this->m_parent->get_node_name(os);

                auto status = this->m_parent->get_child_status(this);

                if(status == child_status::left_child)
                    os << " [style = dashed, color = red] ; " << nl; 
                else if (status == child_status::right_child) 
                    os << " [style = dashed, color = blue] ; " << nl; 
            }

            // root -> left
            if(this->m_left)
            {
                get_node_name(os) << " -> ";
                this->m_left->get_node_name(os) << " [ color = red ] ;" << nl;

                // recursion for left children
                this->m_left->print_node(os);
            }

            // root -> right
            if(this->m_right)
            {
                get_node_name(os) << " -> ";
                this->m_right->get_node_name(os) << " [color = blue ] ; " << nl;
                
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

        const ElementType& get() const { return this->m_value; }

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

        // if insertion fails, returns false

        template<typename Type>
            // we enable this function only when Type and ElementType are same
        enable_if_all_types_are_the_same_t<bool, ElementType, Type>
        insert(Type&& value)
        {
            if(value == this->m_value)
                return false;
            else if(value < this->m_value)
            {
                if(this->m_left)
                    // this is recursion
                    return this->m_left->insert(std::forward<Type>(value));
                else
                {
                    // a new node is inserted and set to this->m_left member
                    // new inserted node is a Leaf node.
                    this->m_left.reset(new binary_node{std::forward<Type>(value), this} );
                    
                    // this call updates all m_height members of its parents
                    this->m_left->update_height();

                    return true;
                }
            }
            else // value > this->m_value
            {
                if(this->m_right)
                    // this is also recursion
                    return this->m_right->insert(std::forward<Type>(value));
                else
                {
                    // a new node is inserted and set to this->m_right member
                    // the new inserted node is a LEAF node
                    this->m_right.reset( new binary_node{std::forward<Type>(value), this});

                    // update all m_height members of its parents
                    this->m_right->update_height();

                    return true;
                }
            }
        }

        // node_ptr_t == std::unique_ptr<binary_node>
        bool graft(node_ptr_t& node_ptr) 
        {
            if(node_ptr->m_value < this->m_value)
            {
                if(this->m_left)
                    // this is recursion
                    return this->m_left->graft(node_ptr);
                else
                {
                    node_ptr->m_parent = this;
                    this->m_left = std::move(node_ptr);
                    this->m_left->update_height();

                    return true;
                }
            }
            else // node_ptr->m_value > this->m_value
            {
                if(this->m_right)
                    // this is also recursion
                    return this->m_right->graft(node_ptr);
                else
                {
                    node_ptr->m_parent = this;
                    this->m_right = std::move(node_ptr);
                    this->m_right->update_height();

                    return true;
                }
            }
        }

        // return true only if all parameters are inserted
        // successfully, otherwise returns false
        template<typename Type, typename... Types>
            // we enable this function only when ElementType, Type, and Types... are
            // of same type
        enable_if_all_types_are_the_same_t<bool, ElementType, Type, Types...>
        insert(Type&& arg, Types&&... args)
        {
            bool result = this->insert(std::forward<Type>(arg));

            if constexpr(sizeof...(args) != 0)
            {
                                    // recursion
                return result && this->insert(std::forward<Types>(args)...);
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
                    os << this->m_value << ", ";

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

                    os << this->m_value << ", ";

                    return;
                }

                case visit_mode::descending_order:
                {
                    if(this->m_right)
                        this->m_right->visit_nodes(os, order);

                    os << this->m_value << ", ";

                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    return;
                }

                case visit_mode::ascending_order:
                default: // in_order
                {
                    if(this->m_left)
                        this->m_left->visit_nodes(os, order);

                    os << this->m_value << ", ";
                    
                    if(this->m_right)
                        this->m_right->visit_nodes(os, order); 

                    return;
                }
            }
        }

        // if fails, return nullptr
        binary_node* find(ElementType value, find_mode fmode,
            visit_mode vmode = visit_mode::ascending_order)
        {
            binary_node* ptr = nullptr;

            if(((vmode==visit_mode::ascending_order) && (fmode==find_mode::successor))||
                ((vmode == visit_mode::descending_order) && (fmode==find_mode::predecessor)))
            {
                    if(this->m_left && (ptr = this->m_left->find(value, fmode, vmode)))
                        return ptr;

                    if( value < this->m_value)
                        return this;

                    if(this->m_right && (ptr = this->m_right->find(value, fmode, vmode)))
                        return ptr;

                    // we failed
                    return ptr;
            }
            else if(( (vmode == visit_mode::ascending_order)&&(fmode==find_mode::predecessor)) ||
                    ((vmode == visit_mode::descending_order)&&(fmode==find_mode::successor)))
            {
                     if(this->m_right && (ptr = this->m_right->find(value, fmode, vmode)))
                        return ptr;

                    if(value > this->m_value)
                        return this;

                     if(this->m_left && (ptr = this->m_left->find(value, fmode, vmode)))
                        return ptr;

                    return ptr;
                }
            else if(fmode == find_mode::exact_match)
                return this->find(value);
            else
                return nullptr;
        }

        bool is_leaf_node()
        {
            // both m_left and m_right are nullptr
            return (!this->m_left && !this->m_right);
        }

        node_ptr_t release_child(binary_node* ptr)
        {
            if(this->m_left && (ptr == this->m_left.get()))
                return std::move(this->m_left);
            else if(this->m_right && (ptr == this->m_right.get()))
                return std::move(this->m_right);
            else
                return nullptr;
        }

        static bool remove_node(node_ptr_t& root_ptr, ElementType value)
        {
            binary_node* ptr = root_ptr->find(value);

            // node with "value" does not exist
            if(!ptr) return false;

            // if parent is nullptr, then it is root node
            if(!ptr->m_parent)
            {
                // ptr is a root node of type binary_node*
                // root_ptr is a root node of type 
                // std::unique_ptr<binary_node>;

                // we move left child to left_child
                // after move, root_ptr->m_left is invalid
                node_ptr_t left_child 
                    = std::move(root_ptr->m_left);

                // after move, root_ptr->m_right is invalid
                node_ptr_t right_child 
                    = std::move(root_ptr->m_right);

                if(right_child) // new root
                {
                    // because root node does not have its parent.
                    right_child->m_parent = nullptr;

                    // existing root_ptr is destroyed
                    // and right_child is moved to root_ptr
                    // at this point, existing old root is
                    // properly released.
                    root_ptr = std::move(right_child);

                    // do not forget this part
                    if(left_child)
                        root_ptr->graft(left_child);

                    return true;
                }
                else if(left_child) // right_child is nullptr
                                    // left_child is a new root.
                {
                    // because root node does not have its parent.
                    left_child->m_parent = nullptr;

                    // after move, the older root is released
                    // and left_child is moved to root_ptr
                    root_ptr = std::move(left_child);

                    return true;
                }
                else
                {
                    // since we destroyed the root
                    // we should not access the root after this statment
                    root_ptr.reset();

                    return true;
                }
            }
            else // non-root node
            {
                // ptr is the node to remove
                // is not a root node.

                if(ptr->is_leaf_node())
                {
                    // this code cuts off the relation
                    // between the child node and its parent.
                    node_ptr_t orphan = 
                        ptr->m_parent->release_child(ptr);

                    // when the orphan goes off this block,
                    // the allocated memory for orphan is
                    // cleaned up. and orphan.get() == ptr

                    if(ptr->m_parent->is_leaf_node())
                    {
                        ptr->m_parent->m_height = 1;
                        ptr->m_parent->update_height();
                    }

                    return true;
                }
                else // not a leaf node, 
                {
                    // cut off the relationship between ptr
                    // and its parent.
                   node_ptr_t orphan =
                    ptr->m_parent->release_child(ptr);

                    // this is for update the height of the parents
                    // nodes
                    ptr->m_parent->height(true);
                    ptr->m_parent->update_height();

                    node_ptr_t left_child =
                        std::move(orphan->m_left);

                    node_ptr_t right_child =
                        std::move(orphan->m_right);

                    if(right_child)
                        root_ptr->graft(right_child);

                    if(left_child)
                        root_ptr->graft(left_child);

                    return true;
                }
            }
        }
}; // end of binary_node class

// std::unique_ptr<binary_node<ElementType>>
template<typename ElementType>
using node_ptr_t = typename binary_node<ElementType>::node_ptr_t;

template<typename ElementType>
void test_remove_node(node_ptr_t<ElementType>& node_ptr, ElementType value)
{
    using binary_node_t = binary_node<ElementType>;
    using node_ptr_t = std::unique_ptr<binary_node_t>;

    binary_node_t::remove_node(node_ptr, value);

    if(node_ptr)
    {
        stream << node_ptr->build_digraph() << endl;
    }
    
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

void print_usage(string_t appname, const char* msg)
{
    stream << "Message: " << msg << endL;

    stream << "Usage 1: " << appname << " graph node_list " << endL;
    stream << "\tExample: " << appname <<" graph 10 8 15 7 9 12 17 6 16 18 > binary_tree.gv" << endL;
    stream << "\tdot -Tpng binary_tree.gv -o binary_tree.png" << endL;

    stream << "Usage 2: " << appname << " list node_list " << endL;
    stream << "\tExample: " << appname <<" list 10 8 15 7 9 12 17 6 16 18" << endL;

    stream << "Usage 3: " << appname << " {ascending|desecnding} {successor_of|predecessor_of|exact_match_of} value node_list " << endL;
    stream << "\tExample: " << appname <<" ascending successor_of 19 10 8 15 7 9 12 17 6 16 18" << endL;
    stream << "\t\t" <<" Finds the successor of 19 in ascending order from the list 10 8 15 7 9 12 17 6 16 18" << endL;

    stream << "Usage 4: " << appname << " remove value node_list " << endL;
    stream << "\tExample: " << appname <<" remove 19 10 8 15 7 9 12 17 6 16 18" << endL;
    stream << "\t\t" <<" Find and remove node with value 19 from the list 10 8 15 7 9 12 17 6 16 18" << endL;
}

int main(int argc, const char* argv[])
{
    // test_ascending_descending_order();
    // test_find_binary_tree();
    // test_build_digraph();

    string_t appname{argv[0]};

    if(argc < 2)
    {
        print_usage(appname, "Program commandline syntax");

        return 0;
    }

    string_t command{argv[1]};

    if(command == "graph")
    {
        binary_node<int> root { std::atoi(argv[2])};

        for(int i = 3; i < argc; ++i)
        {
            root.insert( std::atoi(argv[i]) );
        }
        
        stream << root.build_digraph() << endl;
    }
    else if(command == "list")
   {
        binary_node<int> root { std::atoi(argv[2])};
        using visit_mode = binary_node<int>::visit_mode;

        for(int i = 3; i < argc; ++i)
        {
            root.insert( std::atoi(argv[i]) );
        }
       
        stream <<"Ascending order: ";
        root.visit_nodes(stream, visit_mode::ascending_order);
        stream << endl;

        stream <<"Descending order: ";
        root.visit_nodes(stream, visit_mode::descending_order);
        stream << endl;
    }
     else if(command == "ascending"
        || command == "descending")
    {
        using visit_mode = binary_node<int>::visit_mode;
        using find_mode = binary_node<int>::find_mode;

        visit_mode vmode {visit_mode::undefined};
        find_mode fmode {find_mode::undefined};

        if(command == "ascending")
            vmode = visit_mode::ascending_order;

        if(command == "descending")
            vmode = visit_mode::descending_order;

        if(vmode==visit_mode::undefined)
        {
            print_usage(appname, "Undefined visit mode");
            return 0;
        }

        if(argc < 5)
        {
            print_usage(appname, "Syntax Error");
            return 0;
        }

        string_t operation{argv[2]};

        if(operation == "successor_of")
            fmode = find_mode::successor;

        if(operation == "predecessor_of")
            fmode = find_mode::predecessor;

        if(operation == "exact_match_of")
            fmode = find_mode::exact_match;

        if(fmode == find_mode::undefined)
        {
            print_usage(appname, "Undefined find mode");
            return 0;
        }

        int value = std::atoi(argv[3]);

        binary_node<int> root { std::atoi(argv[4])};
        using visit_mode = binary_node<int>::visit_mode;

        for(int i = 5; i < argc; ++i)
        {
            root.insert( std::atoi(argv[i]) );
        }
       
        stream <<"Ascending order: ";
        root.visit_nodes(stream, visit_mode::ascending_order);
        stream << endl;

        stream <<"Descending order: ";
        root.visit_nodes(stream, visit_mode::descending_order);
        stream << endl;

        auto ptr = root.find(value, fmode, vmode);

        if(ptr)
        {
            if(fmode == find_mode::successor)
                stream << "The successor of " << value;
            else if (fmode == find_mode::predecessor)
                stream << "The predecessor of " << value;
            else if (fmode == find_mode::exact_match)
                stream << "The exact match of " << value;

            if(vmode == visit_mode::ascending_order)
                stream << " in ascending order is ";

            if(vmode == visit_mode::descending_order)
                stream << " in descending order is ";

            stream << ptr->get() << endL;
        }
        else
        {
            if(fmode == find_mode::successor)
                stream << "The successor of " << value;
            else if (fmode == find_mode::predecessor)
                stream << "The predecessor of " << value;
            else if (fmode == find_mode::exact_match)
                stream << "The exact match of " << value;

            if(vmode == visit_mode::ascending_order)
                stream << " in ascending order is NOT found";

            if(vmode == visit_mode::descending_order)
                stream << " in descending order NOT found ";

            stream << endL;
        }
    }
    else if(command == "remove")
    {
        using binary_node_t = binary_node<int>;
        using node_ptr_t = std::unique_ptr<binary_node_t>;

        // binary_tree remove 10 10
        if(argc < 4)
        {
            print_usage(appname, "Syntax Error");
            return 0;
        }
        
        int value = std::atoi(argv[2]);

        node_ptr_t root = std::make_unique<binary_node_t>(std::atoi(argv[3]));
        
        for(int i = 4; i < argc; ++i)
        {
            root->insert( std::atoi(argv[i]) );
        }
        
        test_remove_node(root, value);
    }
}
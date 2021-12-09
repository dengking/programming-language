#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto endL = tpf::endL;

using string_t = std::string;

template<typename ElementType>
class node
{
    public:
    
    using node_ptr_t = std::unique_ptr<node>;

    private:
        ElementType m_value;

        node_ptr_t m_left;
        node_ptr_t m_right;

    public:
        node(ElementType value=ElementType{}):
            m_value{value} { }

        node_ptr_t& left() { return this->m_left; }
        const node_ptr_t& left() const { return this->m_left; }

        node_ptr_t& right() { return this->m_right; }
        const node_ptr_t& right() const { return this->m_right; }

        node& insert(ElementType value)
        {
            if(value < this->m_value)
            {
                if(this->m_left)
                    this->m_left->insert(value);
                else
                    this->m_left.reset(new node{value});                
            }
            else if(value > this->m_value)
            {
                if(this->m_right)
                    this->m_right->insert(value);
                else
                    this->m_right.reset(new node{value});
            }

            return *this;
        }

        string_t get_node_name()
        {
            std::stringstream os;
            os<< "node_" << this->m_value;
            return os.str();
        }

        string_t get_node_definition()
        {
            std::stringstream os;
            
            os << get_node_name()
                << " [ shape=oval, label=\" "
                << this->m_value << " \"]";
            
            return os.str();
        }

        void print_node(std::stringstream& os)
        {
            if(this->m_left)
            {
                os << get_node_name() << " -> " 
                    << this->m_left->get_node_name() << ";" << std::endl;

                this->m_left->print_node(os);
            }

            if(this->m_right)
            {
                os << get_node_name() << " -> "
                    << this->m_right->get_node_name() <<  ";" << std::endl;

                this->m_right->print_node(os);
            }
           
           os << get_node_definition() << std::endl;
        }

        string_t build_digraph()
        {
            std::stringstream os;
            os << "digraph G { "<< std::endl;
            
            print_node(os);

            os <<" }";

            return os.str();
        }
        
        void print(bool bLeft=true)
        {
            if(bLeft)
            {
                if(this->m_left)
                   this->m_left->print(bLeft);
                            
                stream << "{ " << this->m_value << ", "
                    << this->m_balance << " }" << endl;

                if(this->m_right)
                    this->m_right->print(bLeft);
            }
            else
            {
                if(this->m_right)
                    this->m_right->print(bLeft);

                stream << "{ " << this->m_value << ", "
                    << this->m_balance << " }" << endl;

                if(this->m_left)
                    this->m_left->print(bLeft);
            }
        }
};

 int main(int argc, const char* argv[])
 {
    if(argc < 2)
    {
        stream << "Usage: " << argv[0] << " [list of node values] " << endl;
        return 0;
    }
    
    node<int> root{std::atoi(argv[1])};

    for(int i=2; i < argc; ++i)
    {
        root.insert(std::atoi(argv[i]));  
    }

    stream << root.build_digraph() << endl;

    return 0;
 }
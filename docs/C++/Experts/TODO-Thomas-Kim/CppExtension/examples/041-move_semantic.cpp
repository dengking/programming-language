#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

class CopyOrMove
{
    private:
        int m_member;

    public:
        CopyOrMove(int m=int{}): m_member{ m }
        {
            stream << "Default constructor called" << endl;
        }

        CopyOrMove(const CopyOrMove& right_hand_side): 
            m_member{right_hand_side.m_member}
        {
            stream << "Copy Constructor called" << endl;            
        }

        int* operator&() { return &this->m_member; }

        CopyOrMove& operator = (const CopyOrMove& right_hand_side)
        {
            stream << "Copy operator called" << endl;
            
            if(this != std::addressof(right_hand_side))
            {
                this->m_member = right_hand_side.m_member;
            }

            return *this;
        }

        CopyOrMove(CopyOrMove&& right_hand_side)
            : m_member{ std::move(right_hand_side.m_member) }
        {
            stream << "Move constructor callled" << endl;
        }

        CopyOrMove& operator=(CopyOrMove&& right_hand_side)
        {
            stream << "Move assignment called" << endl;

            if(this != std::addressof(right_hand_side))
            {
                this->m_member = std::move(right_hand_side.m_member);
            }

            return *this;
        }        

        ~CopyOrMove()
        {
            stream << "Destructor called"<< endl;
        }
};

CopyOrMove make_copy_or_move(int n)
{
    CopyOrMove obj{n};

    return obj;
}

void don_t_do_this()
{
    std::vector<CopyOrMove> container(2);

    for(size_t i = 0; i < container.size(); ++i)
        container[i] = CopyOrMove{ (int)i };
}

void better_but_not_perfect()
{
    std::vector<CopyOrMove> container; container.reserve(2);

    for(size_t i = 0; i < container.capacity(); ++i)
        container.emplace_back( CopyOrMove{ (int)i } );
}

void do_this()
{
    std::vector<CopyOrMove> container; container.reserve(2);

    for(size_t i = 0; i < container.capacity(); ++i)
        container.emplace_back( (int)i );
}

int main()
{
    // auto o = make_copy_or_move(5);
    don_t_do_this();
    
    stream << endl << endl;

    do_this();

    stream << endl << endl;


    or_do_this();
}


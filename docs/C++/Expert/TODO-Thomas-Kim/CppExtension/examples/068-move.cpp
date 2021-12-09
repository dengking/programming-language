#include <tpf_output.hpp>

using move_type_t = std::unique_ptr<int>;

tpf::sstream stream;
auto endl = tpf::endl;

class ClsType
{
    public:
        mutable move_type_t m_member;
        
    public:
    ClsType() { }

    ClsType(int value): 
        m_member{ new int{value} } { }
    
    move_type_t get_member() 
    { 
        stream << "Type m_member: " << 
            Tpf_GetTypeCategory(this->m_member) << endl;
        
        return std::move(this->m_member); 
    }
};


move_type_t make_member(int v)
{
   ClsType obj(6);
        
    return std::move(obj.m_member);
}

void get_move_type(move_type_t mt)
{

}

int main()
{
    move_type_t m1 {new int{5}};

    get_move_type(m1);
    
}
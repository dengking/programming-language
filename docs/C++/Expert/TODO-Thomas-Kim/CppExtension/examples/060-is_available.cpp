#include <tpf_output.hpp>
#include <functional>

tpf::sstream stream;
auto endl = tpf::endl;

void test_is_available_fn()
{
    using vctr_t = std::vector<int>;
    using deque_t = std::deque<int>;
    
    stream << "is_erase_available_v<std::vector>: " 
        << tpf::types::is_erase_available_v<vctr_t> << endl;

    stream << "is_reserve_available_v<std::vector>: " 
        << tpf::types::is_reserve_available_v<vctr_t> << endl;

    stream << "is_empty_available_v<std::vector>: " 
        << tpf::types::is_empty_available_v<vctr_t> << endl;

    stream << "is_resize_available_v<std::vector>: " 
        << tpf::types::is_resize_available_v<vctr_t> << endl;

    stream << "is_erase_available_v<std::deque>: " 
        << tpf::types::is_erase_available_v<deque_t> << endl;

    stream << "is_reserve_available_v<std::deque>: " 
        << tpf::types::is_reserve_available_v<deque_t> << endl;

    stream << "is_empty_available_v<std::deque>: " 
        << tpf::types::is_empty_available_v<deque_t> << endl;

    stream << "is_resize_available_v<std::deque>: " 
        << tpf::types::is_resize_available_v<deque_t> << endl;
}

int main()
{
    stream 
        << is_member_function_available_v<&std::vector<int>::empty> << endl; 
}
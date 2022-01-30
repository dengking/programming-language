#include <tpf_types.hpp>
#include <tpf_output.hpp>
#include <tpf_set.hpp>

void examples_set()
{
    using set_tag = tpf::set_tag<int, std::deque>;

    tpf::ostream stream;
    auto nl = "\n";
    
    using element_t = set_tag::element_t;
    using set_t = set_tag::set_t;
    using sets_t = set_tag::sets_t;
    using set_of_sets_t = set_tag::set_of_sets_t;
    using sets_of_sets_t = set_tag::sets_of_sets_t;

   set_t A{0, 1, 2, 3, 4};

   stream << tpf::set::build_subsets(A, 3) << nl;
   
}



int main()
{
    examples_set();
}
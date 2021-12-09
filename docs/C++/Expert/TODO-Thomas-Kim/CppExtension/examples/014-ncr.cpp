#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

using namespace tpf;
using namespace tpf::ncrnpr;

void examples_ncr_npr()
{
    sstream stream;
    auto nl = output::nl();
    auto nL = output::nL();

    using ncr_t = combination;

    ncr_t cb{7, 3};
    
    stream << cb << " - " << ncr(7, 3) << nL;
    
    stream << cb.increment_nr() << " - " << ncr(8, 4) << nl;
    
    stream << cb.decrement_nr() << " - " << ncr(7, 3) << nl;
    
    stream << cb.increment_n() << " - " << ncr(8, 3) << nl;
    
    stream << cb.decrement_n() << " - " << ncr(7, 3) << nl;
    
    stream << cb.increment_r() << " - " << ncr(7, 4) << nl;
    
    stream << cb.decrement_r() << " - " << ncr(7, 3) << nl;

}
void examples_enum_combination()
{
    sstream stream;
    auto nl = output::nl();
    auto nL = output::nL();
    
    std::vector<std::string> vct_fruits{"apple", "banana", "cherry"};
    // vct_fruits = tpf::reverse_order(vct_fruits);

    std::deque<std::string> dqe_fruits{"apple", "banana", "cherry"};

    std::list<std::string> lst_fruits{"apple", "banana", "cherry"};
    
    int n = (int)vct_fruits.size();

    for(int r = 0; r <= n; ++r)
    {
        auto max = ncr(n, r);

        for(size_t m_th = 0; m_th < max; ++m_th)
        {
            stream << enumerate_combination<std::vector>(m_th, dqe_fruits, r) << " - "
                    << enumerate_combination<std::vector>(m_th, vct_fruits, r) << " - "
                    << enumerate_combination<std::vector>(m_th, lst_fruits, r) <<nl;
        }
    }
}

void examples_enum_permutations()
{
    sstream stream;
    auto nl = output::nl();
    auto nL = output::nL();

    size_t n = 4;

    for(size_t r = 0; r <= n; ++r)
    {
        auto max = npr(n, r);

        for(size_t m_th = 0; m_th < max; ++m_th)
        {
            stream << enum_permutation(n, r, m_th) 
                << " - " << enum_permutation_list(n, r, m_th) << nl;
        }
    }
}

void examples_enumerate_permutation()
{
    sstream stream;
    auto nl = output::nl();
    auto nL = output::nL();

    std::vector<std::string> vct_fruits{"apple", "banana", "cherry"};
    
    std::deque<std::string> dqe_fruits{"apple", "banana", "cherry"};

    size_t n = vct_fruits.size();

    for(size_t r = 0; r <= n; ++r)
    {
        auto max = npr(n, r);

        for(size_t m_th = 0; m_th < max; ++m_th)
        {
            stream << enumerate_permutation(m_th, vct_fruits, r) 
                << " - " << enumerate_permutation(m_th, dqe_fruits, r) << nl;
        }
    }
}

int main()
{
    sstream stream;

    auto nl = output::nl();
    auto nL = output::nL();
    
    conversion::load_default_locale();

    // examples_enum_permutation();
    // examples_enum_combination();
    examples_enumerate_permutation();

    std::vector<std::string> v{"apple", "banana", "peach"};

    std::vector<std::reference_wrapper<std::string>> vr{v[0], v[1], v[2]};

    stream << vr <<" - 사랑해요!! 나는 김창희" << nl;

}
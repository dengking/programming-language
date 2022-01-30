#include <tpf_output.hpp>
#include <tpf_ncrnpr.hpp>

// #include<ios>
tpf::sstream stream;
auto endl = tpf::endl;
auto endL = tpf::endL;

using element_t = size_t;
using set_t = std::deque<element_t>;

void build_permutation(set_t S, set_t& R, size_t r, size_t m_th)
{
    if(S.empty() || r == 0) return;
    
    auto n_1_p_r_1 = tpf::ncrnpr::npr(S.size()-1, r-1);

    auto quotient = m_th / n_1_p_r_1;
    
    R.push_back(S[quotient]);

    S.erase(S.begin() + quotient);

    build_permutation(S, R, r-1, m_th % n_1_p_r_1);
}

void test_permutation(size_t n, size_t r)
{
    set_t S(n);

    std::iota(S.begin(), S.end(), 1);
    
    stream << "  The set S : "<< S << endL;
    
    auto max_m_th = tpf::ncrnpr::npr(n, r);
    
    for(size_t m_th = 0; m_th < max_m_th; ++m_th)
    {
        set_t R;
        build_permutation(S, R, r, m_th);

        stream <<" m_th -> "<< std::setw(2) << m_th <<" : " << R << endl;
    } 
} 

int main()
{
    // we define a set S = { 1, 2, 3, 4};
    // from this set, we choose 4 elements and enumerate all permutations
    test_permutation(4, 4);

    stream << endL;

    // from a set S with 4 elements,
    // we choose 3 elements and enumerate all permutations

    test_permutation(4, 3);
    
}


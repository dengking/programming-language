#include <tpf_output.hpp>
#include <tpf_ncrnpr.hpp>

tpf::sstream stream;
auto endl = tpf::endl; // one carriage return and flush to console output
auto endL = tpf::endL; // two carriage return and flush to console output

using element_t = size_t;
using set_t = std::vector<element_t>;

void build_permutation(set_t S, set_t& R, size_t r, size_t m_th)
{
    if(r == 0 || S.empty())
        return;

    auto n = S.size(); // n = 4, r = 3

    auto n_1_p_r_1 = tpf::ncrnpr::npr(n-1, r-1); // 3P2 = 6

    auto quotient = m_th / n_1_p_r_1; // index of S
        // == 1 = (7 / 6)
    
    R.push_back (  S[quotient] ); // insert quotient-th element of S to R
    
    //      0, 1, 2, 3 <- quotient
    // S = {1, 2, 3, 4}, S' = { 1, 3, 4}
    S.erase(S.begin() + quotient); // remove S[quotient] from S

    m_th %= n_1_p_r_1; // m_th = m_th % n_1_p_r_1, 7 %= 6
    // m_th = 1

    build_permutation(S, R, r-1, m_th);
}

void test_permutations(size_t n, size_t r)
{
    set_t S(n); // allocate memory for n elements
    std::iota(S.begin(), S.end(), 1);

    stream << " The set S : " << S << endL;

    auto max_m_th = tpf::ncrnpr::npr(n, r);

    for(size_t m_th = 0; m_th < max_m_th; ++m_th)
    {
        set_t R; // permutation

        build_permutation(S, R, r, m_th);

        stream << " m_th -> " << std::setw(3) << m_th << " : " 
            << R << endl;
    }

}

int main()
{
    test_permutations(4, 4);

    stream << endL;

    test_permutations(4, 3);

    stream << endL;

    test_permutations(4, 2);

    stream << endL;

    test_permutations(4, 1);
   
    stream << endL;

    test_permutations(4, 0);
   

}
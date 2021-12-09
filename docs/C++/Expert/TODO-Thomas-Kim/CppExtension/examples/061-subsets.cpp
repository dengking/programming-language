#include <tpf_output.hpp>
#include <tpf_ncrnpr.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto endL = tpf::endL;

using element_t = size_t;
using set_t = std::deque<element_t>;
using sets_t = std::deque<set_t>;

void build_subset(set_t& R, set_t S, size_t r, size_t m_th)
{
    auto n = S.size();

    if(r == 0) 
        return;
    else if(n == r)
    {
        R.insert(R.end(), 
            std::make_move_iterator(S.begin()),
            std::make_move_iterator(S.end()));
        
        return;
    }

    auto n_1_C_r_1 = tpf::ncrnpr::ncr(n-1, r-1);

    if(m_th < n_1_C_r_1 )
    {
        R.push_back(S.front()); // copy the first element of the set S
                                // to the end of the set R

        S.pop_front(); // remove the first element of the set S
        
        --r; // We have chosen one element,
             // so, decrement r

        build_subset(R, S, r, m_th);
    }
    else
    {
        S.pop_front(); 
        
        m_th -= n_1_C_r_1;
        
        build_subset(R, S, r, m_th);
    }
}

void test_build_subset(size_t n, size_t r)
{
    set_t S(n);
    
    std::iota(S.begin(), S.end(), 1);

    stream <<"The set S: " << S << endL;

    auto ncr = tpf::ncrnpr::ncr(n, r);

    for(size_t m_th = 0; m_th < ncr; ++m_th)
    {
        set_t R;
        build_subset(R, S, r, m_th);

        stream << "m_th -> "<< m_th <<" : " << R << endl;
    }
}

int main()
{
    // from a set S = { 1, 2, 3, 4}
    // we choose 2 elements and make subsets
    test_build_subset(4, 2);

    stream << endL;

    // from a set S = {1, 2, 3, 4, 5}
    // we choose 2 elements and make subsets
    test_build_subset(5, 2);

    return 0;
}
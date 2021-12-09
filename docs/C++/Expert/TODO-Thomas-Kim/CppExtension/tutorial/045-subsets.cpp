#include <tpf_output.hpp>
#include <tpf_ncrnpr.hpp>

tpf::sstream stream;
auto endl = tpf::endl; // one carriage return and flush to console
auto endL = tpf::endL; // two carriage returns and flush to console

using element_t = size_t;
using set_t = std::deque<element_t>;

void build_subset(set_t S, set_t& R, // is the subset we are going to build
     size_t r, size_t m_th)
{
    if(r==0)
        return;
    else if(S.size() == r) // S = {1, 2}, r = 2, then we append element of S to R
    {
        R.insert( R.end(), 
            std::make_move_iterator(S.begin()),
            std::make_move_iterator(S.end()));
        
        return;
    }

    // nCr = n-1 C r-1 + n-1 C r

    auto n_1_c_r_1 = tpf::ncrnpr::ncr(S.size()-1, r-1);

    if(m_th < n_1_c_r_1)
    {
        // for the first group

        R.push_back(S.front()); // copy the first element of S to R
        S.pop_front(); // remove the first element of S
        
        --r; // Since we determined the first element of the subset R,
                // we decrement r.

        build_subset(S, R, r, m_th);
    }
    else
    {
        // for the second group

        // we remove the first element of S
        S.pop_front(); 
        
        // but, we do not add the first element of S to R
        m_th -= n_1_c_r_1;

        build_subset(S, R, r, m_th);
    }

}

void test_subsets(size_t n, size_t r)
{
    set_t S(n); // allocate memory for n elements

    std::generate(S.begin(), S.end(), 
        [n = size_t{} ]() mutable{ return ++n; });

    // std::iota(S.begin(), S.end(), 
       // [n = size_t{} ]() mutable{ return ++n; });

    stream << "The set S : " << S << endL;

    // total number of subsets 
    auto max_m_th = tpf::ncrnpr::ncr(n, r);

    for(size_t m_th = 0; m_th < max_m_th; ++m_th)
    {
        set_t R;

        build_subset(S, R, r, m_th);

        stream << "m_th - > " << m_th << " : " << R << endl;
    }

}

int main()
{
    test_subsets(4, 2);

    stream << endL;

    test_subsets(8, 4);

}
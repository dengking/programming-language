#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

void examples_for_permutations()
{
    using namespace tpf::ncrnpr;
    using namespace tpf::output;

    tpf::console cout;
    auto nl = tpf::output::nl();

    using m_th_t = tpf::ncrnpr::permutation::number_t;

    m_th_t n;
    cout << "input n: "; std::cin >> n;

    auto n_p_r = npr(n, n);

    cout << "Computing ... " <<  n_p_r << " permutations!" << nl;
    for(m_th_t i = 0; i <= n_p_r; ++i)
       {

         // enum_permutation((m_th_t)12, (m_th_t)12, i);
       }

    cout << "Done..." << nl;
    
}

int main()
{
    examples_for_permutations();
}
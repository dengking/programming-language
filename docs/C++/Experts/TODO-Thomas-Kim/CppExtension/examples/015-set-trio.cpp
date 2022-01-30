#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;
namespace ncrnpr = tpf::ncrnpr;
namespace output = tpf::output;
namespace conversion = tpf::conversion;

void examples_set_alias()
{
    tpf::console stream;
    auto nl = output::nl();
    auto nL = output::nL();
    
    using set_tag = tpf::set_tag<std::vector, std::string>;
    using set_element_t = tpf::set_element_t<set_tag>;
    using set_t = tpf::set_t<set_tag>;
    using duet_set_t = tpf::duet_set_t<set_tag>;

    set_t people{"Alice", "Benjamine", "Charles", "Dracula"};

    size_t n = people.size();

      ncrnpr::combination combinations{n};


    using m_th_t = ncrnpr::combination::m_th_t;

    for(size_t r=0; r <= n; ++r)
    {
        combinations.set_r(r);

        for(m_th_t m_th = 0; m_th < combinations(); ++m_th)
        {
            stream << ncrnpr::build_combination(m_th, people, r) << nl;
        }
    }   
}

void examples_build_combinations()
{
    tpf::console stream;
    auto nl = output::nl();
    auto nL = output::nL();
    auto newline = "\n";
    
    using set_tag = tpf::set_tag<std::vector, std::string>;
    using set_element_t = tpf::set_element_t<set_tag>;
    using set_t = tpf::set_t<set_tag>;
    using duet_set_t = tpf::duet_set_t<set_tag>;

    set_t people{"Alice", "Benjamine", "Charles", "Dracula"};


    auto sets = ncrnpr::build_combination_indices<std::vector>(5);

    for(auto& duet: sets)
    {
        auto& [selected_set, complement_set] = duet;

        stream << selected_set << " - " << complement_set << newline;
    }

    stream << nl;   
}


void examples_build_combinations_include()
{
    tpf::console stream;
    auto nl = output::nl();
    auto nL = output::nL();
    auto newline = "\n";
    
    using set_tag = tpf::set_tag<std::vector, size_t>;
    using set_element_t = tpf::set_element_t<set_tag>;
    using set_t = tpf::set_t<set_tag>;
    using duet_set_t = tpf::duet_set_t<set_tag>;

    set_t include_set{2, 3, 5};


    auto sets = ncrnpr::build_combination_including_indices<std::vector>
                                            (include_set, 9);

    for(auto& duet: sets)
    {
        auto& [selected_set, complement_set] = duet;

        stream << selected_set << " - " << complement_set << newline;
    }

    stream << nl;   
}

void examples_build_combinations_exclude()
{
    tpf::console stream;
    auto nl = output::nl();
    auto nL = output::nL();
    auto newline = "\n";
    
    using set_tag = tpf::set_tag<std::vector, size_t>;
    using set_element_t = tpf::set_element_t<set_tag>;
    using set_t = tpf::set_t<set_tag>;
    using duet_set_t = tpf::duet_set_t<set_tag>;

    set_t exclude_set{1, 3, 5, 7, 9};


    auto sets = ncrnpr::build_combination_excluding_indices<std::vector>
                                            (exclude_set, 9);

    for(auto& duet: sets)
    {
        auto& [selected_set, complement_set] = duet;

        stream << selected_set << " - " << complement_set << newline;
    }

    stream << nl;   
}


int main()
{
   // examples_build_combinations();
   // examples_build_combinations_include();
   examples_build_combinations_exclude();
}
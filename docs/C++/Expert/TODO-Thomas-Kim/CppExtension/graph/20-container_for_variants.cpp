

#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

template<typename Type>
auto to_ref(const Type& value)
{
    return std::ref(const_cast<Type&>(value));
}

void how_to_filter_containers_for_variants()
{
    using age_t = int;
    using weight_t = double;
    using name_t = const char*;

    // we defined a container type to hold variants
    using personal_info_t = 
        types::container_of_variants_t<std::vector, age_t, weight_t, name_t>;

   personal_info_t info; // container info of 
                            // std::vector of std::variant<age_t, weight_t, name_t>

   info.emplace_back(10);
   info.emplace_back(20);
   info.emplace_back(40.6);
   info.emplace_back(50.7);
   info.emplace_back("Thomas Kim");
   info.emplace_back("Steve Park");
   
   stream <<"The contents of info: " << info << endl;

//    std::vector<age_t> ages;
//    std::vector<weight_t> weights;
//    std::vector<name_t> names;

//    types::overloads filters
//    {
//        [&ages](const age_t& age) { ages.emplace_back(age); },
//        [&weights](const weight_t& weight) { weights.emplace_back(weight); },
//        [&names](const auto& name) { names.emplace_back(name); }
//    };

   std::vector<std::reference_wrapper<age_t>> ages;
   std::vector<std::reference_wrapper<weight_t>> weights;
   std::vector<std::reference_wrapper<name_t>> names;

//    types::overloads filters
//    {
//        [&ages](const age_t& age) { ages.emplace_back(std::ref(const_cast<age_t&>(age))); },
//        [&weights](const weight_t& weight) { weights.emplace_back(std::ref(const_cast<weight_t&>(weight))); },
//        [&names](const auto& name) { names.emplace_back(std::ref(const_cast<name_t&>(name))); }
//    };

    types::overloads filters
    {
       [&ages](const age_t& age) 
              { ages.emplace_back(to_ref(age)); },
       [&weights](const weight_t& weight) 
            { weights.emplace_back(to_ref(weight)); },
       [&names](const auto& name) 
            { names.emplace_back(to_ref(name)); }
    };

   filters.for_each(info);

   stream <<"\nAges: " << ages << endl;
   stream << "\nWeights: " << weights << endl;
   stream << "\nNames: " << names << endl;
}

int main()
{
    how_to_filter_containers_for_variants();
}
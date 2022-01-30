#include <tpf_output.hpp>

// clang++ -std=c++17 -Xclang -flto-visibility-public-std 32-containers_for_variants.cpp
// g++ -std=c++17 32-containers_for_variants.cpp
// cl /EHsc /std:c++17 32-containers_for_variants.cpp /Fe: a.exe

namespace types = tpf::types;

tpf::sstream stream; // string stream
auto nl = tpf::nl; // "\n"
auto endl = tpf::endl; // for flushing to console oupt

void test_vector_of_variants()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    // container_t = std::vector< std::variant<name_t, age_t, weight_t> >
    // container_t = std::vector< std::variant<std::string, int, double> >
    // container_t = types::vector_of_variants_t<name_t, age_t, weight_t, age_t>;
    using container_t = types::vector_of_variants_t<name_t, age_t, weight_t, age_t>;
    using value_type = typename container_t::value_type; 
    
    stream << Tpf_GetTypeName(value_type) << endl;

    container_t container; // std::vector< std::variant<std::string, int, double> >

    container.emplace_back("Thomas Kim"); 
    container.emplace_back(19);
    container.emplace_back(22.0 /7.0);
    container.emplace_back(30);
    container.emplace_back(21.0 /7.0);
    container.emplace_back("James Park"); 

    auto& stream = ::stream;
    auto& nl = ::nl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements = types::make_variant_visitors
    (
        [&stream, &nl](auto&& name)
        {
            stream << "Name is " << name << nl;
        },
        
        [&stream, &nl](auto&& age)
        {
            stream << "Age is " << age << nl;
        },

        [&stream, &nl](auto&& weight)
        {
            stream << "Weight is " << weight << nl;
        }
    );

    handle_elements.for_each(container);
    stream << endl;

    handle_elements.for_each_reverse(container);
    stream << endl;

    handle_elements.for_each( types::reverse(container) );
    stream << endl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements_with_iterator = types::make_variant_visitors
    (
        [&stream, &nl](auto&& itr, auto&& name)
        {
            stream << "Name is " << name << nl;
            
            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        },
        
        [&stream, &nl](auto&& itr, auto&& age)
        {
            stream << "Age is " << age << nl;
            
            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        },

        [&stream, &nl](auto&& itr, auto&& weight)
        {
            stream << "Weight is " << weight << nl;

            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        }
    );

    handle_elements_with_iterator.for_each_iterator(container);

    stream << endl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements_with_index = types::make_variant_visitors
    (
        [&stream, &nl, &container](auto index, auto&& name)
        {
            stream << index <<" - Name is " << name << nl;
        },
        
        [&stream, &nl, &container](auto index, auto&& age)
        {
            stream << index << " - Age is " << age << nl;
        },

        [&stream, &nl, &container](auto index, auto&& weight)
        {
            stream << index << " - Weight is " << weight << nl;
        }
    );

    handle_elements_with_index.for_each_index(container);
    stream << endl;

    handle_elements_with_index.for_each_reverse_index(container);
    stream << endl;

    handle_elements_with_index.for_each_index(types::reverse(container));
    stream << endl;

    for(auto& vt: container)
    {
        //handle_elements(std::forward<decltype(vt)>(vt));
        handle_elements(vt);
    }

    stream << endl;

    for(auto& vt: types::reverse(container))
    {
        //handle_elements(std::forward<decltype(vt)>(vt));
        handle_elements(vt);
    }
    
    stream << endl;
}

///////////////////////////////////
void test_set_of_variants()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    // container_t = std::set< std::variant<name_t, age_t, weight_t> >
    // container_t = std::set< std::variant<std::string, int, double> >
    // container_t = types::set_of_variants_t<name_t, age_t, weight_t, age_t>;
    using container_t = types::set_of_variants_t<name_t, age_t, weight_t, age_t>;
    using value_type = typename container_t::value_type; 
    
    stream << Tpf_GetTypeName(value_type) << endl;

    container_t container; // std::set< std::variant<std::string, int, double> >

    container.emplace("Thomas Kim"); 
    container.emplace(19);
    container.emplace(22.0 /7.0);
    container.emplace(30);
    container.emplace(21.0 /7.0);
    container.emplace("James Park"); 

    auto& stream = ::stream;
    auto& nl = ::nl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements = types::make_variant_visitors
    (
        [&stream, &nl](auto&& name)
        {
            stream << "Name is " << name << nl;
        },
        
        [&stream, &nl](auto&& age)
        {
            stream << "Age is " << age << nl;
        },

        [&stream, &nl](auto&& weight)
        {
            stream << "Weight is " << weight << nl;
        }
    );

    handle_elements.for_each(container);
    stream << endl;

    handle_elements.for_each_reverse(container);
    stream << endl;

    handle_elements.for_each( types::reverse(container) );
    stream << endl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements_with_iterator = types::make_variant_visitors
    (
        [&stream, &nl](auto&& itr, auto&& name)
        {
            stream << "Name is " << name << nl;
            
            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        },
        
        [&stream, &nl](auto&& itr, auto&& age)
        {
            stream << "Age is " << age << nl;
            
            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        },

        [&stream, &nl](auto&& itr, auto&& weight)
        {
            stream << "Weight is " << weight << nl;

            stream << "Type of iterator: "
                << Tpf_GetTypeCategory(*itr) << nl;
        }
    );

    handle_elements_with_iterator.for_each_iterator(container);

    stream << endl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements_with_index = types::make_variant_visitors
    (
        [&stream, &nl, &container](auto index, auto&& name)
        {
            stream << index <<" - Name is " << name << nl;
        },
        
        [&stream, &nl, &container](auto index, auto&& age)
        {
            stream << index << " - Age is " << age << nl;
        },

        [&stream, &nl, &container](auto index, auto&& weight)
        {
            stream << index << " - Weight is " << weight << nl;
        }
    );

    handle_elements_with_index.for_each_index(container);
    stream << endl;

    handle_elements_with_index.for_each_reverse_index(container);
    stream << endl;

    handle_elements_with_index.for_each_index(types::reverse(container));
    stream << endl;

    for(auto& vt: container)
    {
        //handle_elements(std::forward<decltype(vt)>(vt));
        handle_elements(vt);
    }

    stream << endl;

    for(auto& vt: types::reverse(container))
    {
        //handle_elements(std::forward<decltype(vt)>(vt));
        handle_elements(vt);
    }
    
    stream << endl;
}

void test_vector_of_variant_with_vector()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using vctr_t = std::vector<name_t>;

   // using vctr_vt_t = types::deque_of_variants_t<age_t, weight_t>;
    using vctr_vt_t = types::container_of_variants_t<std::deque, age_t, weight_t>;

    // container_t = std::vector< std::variant<name_t, age_t, weight_t> >
    // container_t = std::vector< std::variant<std::string, int, double> >
    // container_t = types::vector_of_variants_t<name_t, age_t, weight_t, vctr_t>;
    using container_t = types::vector_of_variants_t<name_t, age_t, weight_t, vctr_t, vctr_vt_t>;
    using value_type = typename container_t::value_type; 
    
    // stream << Tpf_GetTypeName(value_type) << endl;

    container_t container; // std::vector< std::variant<std::string, int, double> >

    container.emplace_back("Thomas Kim"); 
    container.emplace_back(19);
    container.emplace_back(22.0 /7.0);
    container.emplace_back(vctr_t{ "Sophie Turner", "Robert Park", "The Programmer"} );
    container.emplace_back(30);
    container.emplace_back(21.1 /7.0);
    container.emplace_back("James Park"); 
    container.emplace_back(vctr_vt_t{ 1, 2, 3, 1.5, 2.3, 4.6 } ); 



    auto& stream = ::stream;
    auto& nl = ::nl;

    // we are creating an instance of types::variant_visitors
    auto handle_elements = types::make_variant_visitors
    (
        [&stream, &nl](auto&& name)
        {
            stream << "Name is " << name << nl;
        },
        
        [&stream, &nl](auto&& age)
        {
            stream << "Age is " << age << nl;
        },

        [&stream, &nl](auto&& weight)
        {
            stream << "Weight is " << weight << nl;
        },
        
        [&stream, &nl](auto&& names) // vctr_t = std::vector<std::string>
        {
            stream << "Names: " << names << nl;
        },

        [&stream, &nl](auto&& vctr_vt) // vctr_vc_t = std::vector<std::variant<age_t, weight_t>>
        {
            types::make_variant_visitors
            (
                [&stream, &nl](auto&& age)
                {
                    stream <<"vctr_vc_t - age: " << age << nl; 
                },
                
                [&stream, &nl](auto&& weight)
                {
                    stream <<"vctr_vc_t - weight: " << weight << nl;
                }

            ).for_each(vctr_vt);
        }
    );

    handle_elements.for_each(container);
    stream << endl;

}

void test_map_of_variants()
{
    using key_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using list_t = std::list<std::string>; 

    // container_t = std::map<key_t, std::variant<name_t, age_t, weight_t>>
    using container_t = types::map_of_variants_t<key_t, list_t, name_t, age_t, weight_t>;

    container_t container;

    container["Programmer"] = "Thomas Kim";
    container["Age"] = 19;
    container["Weight"] = 61.5;

    container["Friend"] = "Sophie Turner";
    container["Her Age"] = 17;
    container["Her Weight"] = 55.5;

    container["Old Buddies"] = list_t{"James", "Amie", "Robert", "Michelle"};
    
    auto& stream = ::stream;
    auto& nl = ::nl;

    auto visitors = types::make_variant_visitors
    (
        [&stream, &nl](auto&& key, auto&& list)
        {
            stream << "\nIn the list: " << nl;

            for(auto& n: list)
            {
                stream << key <<" is " << n << nl;
            }

            stream <<"End of list"<<nl << nl;
        },

        [&stream, &nl](auto&& key, auto&& name)
        {
            stream << key << " is " << name << nl;
        },

        [&stream, &nl](auto&& key, auto&& age)
        {
            stream << key << " is " << age << nl;
        },

        [&stream, &nl](auto&& key, auto&& weight)
        {
            stream << key << " is " << weight << nl;
        }

    );

    visitors.for_each(container);
    // flush to console
    stream << endl;

    visitors.for_each_reverse(container);
    // flush to console
    stream << endl;

    for(auto& vt: container)
    {
        visitors(vt);
    }

    stream << endl;

    auto visitors_with_iterator = types::make_variant_visitors
    (
        [&stream, &nl](auto&& itr, auto&& key, auto&& list)
        {
            // stream << "\nIn the list: " << nl;

            // for(auto& n: list)
            // {
            //     stream << key <<" is " << n << nl;
            // }

            // stream <<"End of list"<<nl << nl;

            stream << "In the list: " << list << nl;
            // stream << "Type of the element: " 
              //  << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& name)
        {
            stream << key << " is " << name << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& age)
        {
            stream << key << " is " << age << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& weight)
        {
            stream << key << " is " << weight << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        }
    );

    visitors_with_iterator.for_each_iterator(container);
    
    stream << endl;
}

//////////////////////////////////////////////////////
void test_unordered_map_of_variants()
{
    using key_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using list_t = std::list<std::string>; 

    // container_t = std::unordered_map<key_t, std::variant<name_t, age_t, weight_t>>
    using container_t = types::unordered_map_of_variants_t<key_t, list_t, name_t, age_t, weight_t>;

    /*
        std::unordered_map does not support
        reverse iterators
     */

    container_t container;

    container["Programmer"] = "Thomas Kim";
    container["Age"] = 19;
    container["Weight"] = 61.5;

    container["Friend"] = "Sophie Turner";
    container["Her Age"] = 17;
    container["Her Weight"] = 55.5;

    container["Old Buddies"] = list_t{"James", "Amie", "Robert", "Michelle"};
    
    auto& stream = ::stream;
    auto& nl = ::nl;

    auto visitors = types::make_variant_visitors
    (
        [&stream, &nl](auto&& key, auto&& list)
        {
            stream << "\nIn the list: " << nl;

            for(auto& n: list)
            {
                stream << key <<" is " << n << nl;
            }

            stream <<"End of list"<<nl << nl;
        },

        [&stream, &nl](auto&& key, auto&& name)
        {
            stream << key << " is " << name << nl;
        },

        [&stream, &nl](auto&& key, auto&& age)
        {
            stream << key << " is " << age << nl;
        },

        [&stream, &nl](auto&& key, auto&& weight)
        {
            stream << key << " is " << weight << nl;
        }

    );

    visitors.for_each(container);
    // flush to console
    stream << endl;

    // visitors.for_each_reverse(container);
    // // flush to console
    // stream << endl;

    for(auto& vt: container)
    {
        visitors(vt);
    }

    stream << endl;

    auto visitors_with_iterator = types::make_variant_visitors
    (
        [&stream, &nl](auto&& itr, auto&& key, auto&& list)
        {
            // stream << "\nIn the list: " << nl;

            // for(auto& n: list)
            // {
            //     stream << key <<" is " << n << nl;
            // }

            // stream <<"End of list"<<nl << nl;

            stream << "In the list: " << list << nl;
            // stream << "Type of the element: " 
              //  << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& name)
        {
            stream << key << " is " << name << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& age)
        {
            stream << key << " is " << age << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        },

        [&stream, &nl](auto&& itr, auto&& key, auto&& weight)
        {
            stream << key << " is " << weight << nl;
            // stream << "Type of the element: " 
            //     << Tpf_GetTypeCategory(*itr) << nl << nl;
        }
    );

    visitors_with_iterator.for_each_iterator(container);
    
    stream << endl;
}


int main()
{
    // test_vector_of_variants();
    
    // test_set_of_variants();

    // test_vector_of_variant_with_vector();

    // test_map_of_variants();

    test_unordered_map_of_variants();
}
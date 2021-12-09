#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

void test_set_of_variants()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;

    using set_t = types::set_of_variants_t<const char*, int, double>;
    
    set_t my_set;

    my_set.emplace("Thomas Kim");
    my_set.emplace(20);
    my_set.emplace(56.0);
    
    tpf::sstream stream;
    auto endl = tpf::endl;

    types::variant_handlers handle_map
    {
        [&stream, &endl](const int& age)
        {
            stream << "I am " << age << " years old. " << endl;
        },

        [&stream, &endl](const double& weight)
        {
            stream << "Weight is " << weight << " kg. " << endl;
        },

        [&stream, &endl](const char* const& name)
        {
            stream << "My name is " << name <<". " << endl;
        },

        [&stream, &endl](auto&& var)
        {
           stream <<"Possibly wrong parameter definition: " 
            << Tpf_GetTypeCategory(var) << endl;
        }
    };

    handle_map.for_each(my_set);
    stream << endl;
}

void test_map_of_variants()
{
    using category_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;
    
    using map_t = 
        types::map_of_variants_t<category_t, name_t, age_t, weight_t>;
    
    map_t info;

    info["Tom's name"] = "Thomas Kim";
    info["Toms' age"] = 20;
    info["Tom's weight"] = 60.5;

    tpf::sstream stream;
    auto endl = tpf::endl;

    types::variant_handlers handle_map
    {
        [&stream, &endl](const category_t& key, name_t& name)
        {
            stream << key << " is " << name << endl;
        },

        [&stream, &endl](const category_t& key, age_t& age)
        {
            stream << key << " is " << age << endl;
        },

        [&stream, &endl](const category_t& key, weight_t& weight)
        {
            stream << key << " is " << weight << endl;
        },

        [&stream, &endl](const category_t& key, auto catch_all)
        {
            stream << "Catch all: " << Tpf_GetTypeCategory(catch_all) << endl;
        }
    };

    handle_map.for_each(info);
    stream << endl;
}

void test_unordered_map_of_variants()
{
    using category_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;
    
    using map_t = 
        types::unordered_map_of_variants_t<category_t, name_t, age_t, weight_t>;
    
    map_t info;

    info["Tom's name"] = "Thomas Kim";
    info["Toms' age"] = 20;
    info["Tom's weight"] = 60.5;

    tpf::sstream stream;
    auto endl = tpf::endl;

    types::variant_handlers handle_map
    {
        [&stream, &endl](const category_t& key, name_t& name)
        {
            stream << key << " is " << name << endl;
        },

        [&stream, &endl](const category_t& key, age_t& age)
        {
            stream << key << " is " << age << endl;
        },

        [&stream, &endl](const category_t& key, weight_t& weight)
        {
            stream << key << " is " << weight << endl;
        },

        [&stream, &endl](const category_t& key, auto catch_all)
        {
            stream << "Catch all: " << Tpf_GetTypeCategory(catch_all) << endl;
        }
    };

    handle_map.for_each(info);
    stream << endl;
}

void test_multi_maps()
{
    using category_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;
    
    using map_t = types::unordered_multimap_of_variants_t<category_t, name_t, age_t, weight_t>;
    
    map_t info;

    info.emplace("Tom's name", "Thomas Kim");
    info.emplace("Toms' age", 20);
    info.emplace("Tom's weight", 60.5);

    for(auto p: info)
    {
        auto& [key, value] = p;

        stream <<key <<" is " << value << endl;
    }
}

void test_handle_unordered_maps()
{
    using category_t = const char*;
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    
    using map_t = types::map_of_variants_t<category_t, age_t, weight_t, name_t>;

    using key_type = map_t::key_type;
    using value_type = map_t::value_type;
        
    map_t info;

    info["Tom's name"] = "Thomas Kim";
    info["Toms' age"] = 20;
    info["Tom's weight"] = 60.5;

    tpf::sstream stream;
    auto endl = tpf::endl;

    types::variant_handlers handle_map
    {
        [&stream, &endl](const category_t& key, name_t& value)
        {
            stream << key << " is " << value << endl;
        },

        [&stream, &endl](const category_t& key, age_t& value)
        {
            stream << key << " is " << value << endl;
        },

        [&stream, &endl](const category_t& key, weight_t& value)
        {
            stream << key << " is "<< value << endl;
        }
   };

    handle_map.for_each(info);
   
}

void test_multimap_of_variants()
{
    using category_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;
    
    using map_t = types::multimap_of_variants_t<category_t, name_t, age_t, weight_t>;
    
    map_t info;

    info.emplace("Tom's name", "Thomas Kim");
    info.emplace("Toms' age", 20);
    info.emplace("Tom's weight", 60.5);

    tpf::sstream stream;
    auto endl = tpf::endl;

    types::variant_handlers handle_map
    {
        [&stream, &endl](const category_t& key, name_t& name)
        {
            stream << key << " is " << name << endl;
        },

        [&stream, &endl](const category_t& key, age_t& age)
        {
            stream << key << " is " << age << endl;
        },

        [&stream, &endl](const category_t& key, weight_t& weight)
        {
            stream << key << " is " << weight << endl;
        }
    };

    handle_map.for_each(info);
    stream << endl;

    handle_map.for_each_reverse(info);
 }


int main()
{
    test_set_of_variants();
    test_map_of_variants();
    test_unordered_map_of_variants();

    test_multimap_of_variants();


}
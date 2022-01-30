#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;
auto endL = tpf::endL;

void powerful_application_of_map_of_variants()
{
    using key_t = const char*;
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using person_t = types::unordered_map_of_variants_t<key_t, name_t, age_t, weight_t>;
    
    // people_t is std::vector<std::map<key_t, std::variant<name_t, age_t, weight_t>>
    using people_t = std::vector<person_t>;

    auto key_name = "name";
    auto key_age = "age";
    auto key_weight = "weight";

    people_t people; // vector of persons

    people.emplace_back(person_t{ {key_name, "Thomas Kim"}, { key_age, 23 }, {key_weight, 56.0} } );
    people.emplace_back(person_t{ {key_name, "Sophie Turner"}, { key_age, 26 }, {key_weight, 66.5} } );
    people.emplace_back(person_t{ {key_name, "Albert Kim"}, { key_age, 21 }, {key_weight, 60.5} } );
    
    for(auto& person: people)
    {
        stream << "My name is " << person[key_name] << ". " << endl;
        stream << "I am " << person[key_age] << " years old." << endl;
        stream << "I weigh " << person[key_weight] << " kg." << endL;
    }
}

int main()
{
    powerful_application_of_map_of_variants();
}
#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

void _1_test_type_functions()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;

    using price_t = float;
    using title_t = const char*;

    using author_t = const char*;
    
    using snack_t = std::tuple<name_t, weight_t>;
    using book_t = std::tuple<title_t, price_t, author_t>;
    using device_t = std::tuple<name_t, price_t, weight_t>;

    using item_t = types::any<snack_t, book_t, device_t>;

    using bag_t = std::vector<item_t>; 

    bag_t bag;

    bag.emplace_back( snack_t{ "Honey choco", 300.0 });

    bag.emplace_back( book_t{ "Good Programmer", 30.0f, "Thomas Kim" });

    bag.emplace_back( device_t{ "Good Programmer", 1000.0f, 1.6 });

    stream << bag << endl;

    for(auto&& e: bag)
    {
        stream << e << endl;
    }
}

void _2_test_print_elements()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;

    using price_t = float;
    using title_t = const char*;
    using author_t = const char*;
    
    using snack_t = std::tuple<name_t, weight_t>;
    using snacks_t = std::vector<snack_t>;

    using book_t = std::tuple<title_t, price_t, author_t>;
    using books_t = std::vector<book_t>;

    using device_t = std::tuple<name_t, price_t, weight_t>;
    using devices_t = std::vector<device_t>;

    using belongings_t = 
        types::any<snack_t, snacks_t, book_t, books_t, device_t, devices_t>;

    belongings_t mine;

    mine = snack_t{"Yummy Choco", 300.0 };

    stream << mine << endl;

    mine = snacks_t{ snack_t{" Yummy Choco", 300}, snack_t{"Sweet Potato", 200} };

    stream << mine << endl;

    mine = books_t{ book_t{"Good Book", 20.0f, "Thomas Kim"},
    book_t{"Poor Book", 20.0f, "Thomas Kim"}, book_t{"Excellent Book", 20.0f, "Thomas Kim"} };

     stream << mine << endl;

}

void _3_test_any_as_tuple_elements()
{

    using cate_t = const char*;
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;

    using price_t = float;
    using title_t = const char*;
    using author_t = const char*;
    
    using book_t = std::tuple<cate_t, title_t, weight_t>;
    using device_t = std::tuple<cate_t, name_t, price_t>;
    using thing_t = types::any<book_t, device_t>;
    
    using person_t = std::tuple<cate_t, thing_t>;

   person_t tom{"person", book_t{ "book", "Good Man", 300 } };

   stream << tom << endl;

   tom = person_t{"person", device_t{"device", "MyPowerLaptop", 1000}  };

    stream << tom << endl;

    auto& [cate, thing] = tom;

    stream << "Person: " << cate << endl;

    thing.if_current_type<book_t>([](auto&& book)
    {
               stream << "I love this book [" << book <<"]" << endl;
        });

    thing.if_current_type<device_t>([](auto&& device)
    {
               stream << "I love this device [" << device <<"]" << endl;
        });
}

void _4_test_any_as_container_elements()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    
    enum {idx_name, idx_age, idx_weight};

    using any_t = types::any<name_t, age_t, weight_t>;

    using tuple_t = std::tuple<name_t, any_t>;

    using container_t = std::vector<tuple_t>;

    container_t things;

    things.emplace_back("name", "Thomas Kim");
    things.emplace_back("age", 30);
    things.emplace_back("weight", 60.0);

    stream << things << endl;

    auto handle_name = [](auto&& value)
    {
        stream << "\""<< value << "\" is my friend!" << endl;
    };

    auto handle_age = [](auto&& value)
    {
        stream << "He is "<< value << " old." << endl;
    };

    auto handle_weight = [](auto&& value)
    {
        stream << "He weighs "<< value << " kilogram." << endl;
    };

    for(auto&& thing: things)
    {
        auto[cate, item] = thing;

        types::handle_any(item, handle_name, handle_age, handle_weight);
    }

    stream << endl << endl;

    for(auto&& thing: things)
    {
        auto[cate, item] = thing;

        item.if_current_type<name_t>(handle_name);
        
        // item.if_current_type<age_t>(handle_age);
        // item.if_current_type<weight_t>(handle_weight);
    }

    for(auto&& thing: things)
    {
        auto[cate, item] = thing;

        types::if_current_index<idx_name>(item, handle_name);
        
        // item.if_current_type<age_t>(handle_age);
        // item.if_current_type<weight_t>(handle_weight);
    }

    
}

int main()
{
    // _1_test_type_functions();

    // _2_test_print_elements();

    // _3_test_any_as_tuple_elements();

    _4_test_any_as_container_elements();
}
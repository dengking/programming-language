#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto nl = tpf::nl;

namespace types = tpf::types;

void test_container_of_variants()
{
    using coin_t = int;
    using money_t = double;
    using title_t = const char*;

    using bag_t = types::vector_of_variants_t<coin_t, money_t, title_t>;
    using item_t = bag_t::variant_type;

    bag_t my_bag; // vector of variants

    my_bag.emplace_back(10);
    my_bag.emplace_back(100.23);
    my_bag.emplace_back("Modern C++ Programming");
    my_bag.emplace_back("C++17 Standard");

    stream <<"Contents of my bag: " << my_bag << endl;

    auto handle_coin = [](const coin_t& coin_count)
    {
        stream <<"There are " << coin_count <<" coins."<<nl;
    };

    auto handle_money = [](const money_t& money)
    {
        stream <<"I have " << money <<" bucks." << nl;
    };

    auto handle_title = [](title_t& title)
    {
        stream <<"I like reading \"" << title << "\"."<<nl;
    };

    types::overloads handle_my_bag { handle_coin, handle_money, handle_title};

    auto handle_items = [&handle_my_bag](auto& item)
    {
        std::visit(handle_my_bag, item);
    };

    stream <<"Items in my bag, item by item: "<< endl;
    
    for(auto& item: my_bag)
    {
        handle_items(item);
    }

    stream << endl;
}

void test_container_of_variants_simplified()
{
    using coin_t = int;
    using money_t = double;
    using title_t = const char*;

    using bag_t = types::vector_of_variants_t<coin_t, money_t, title_t>;
    using item_t = bag_t::variant_type;

    bag_t my_bag; // vector of variants

    my_bag.emplace_back(10);
    my_bag.emplace_back(100.23);
    my_bag.emplace_back("Modern C++ Programming");
    my_bag.emplace_back("C++17 Standard");

    stream <<"Contents of my bag: " << my_bag << endl;

    types::overloads handle_my_bag 
    { 
        [](const coin_t& coin_count)
        {
            stream <<"There are " << coin_count <<" coins."<<nl;
        },

        [](const money_t& money)
        {
            stream <<"I have " << money <<" bucks." << nl;
        },

        [](title_t& title)
        {
            stream <<"I like reading \"" << title << "\"."<<nl;
        }
        
    }; // constructor of types::overloads
   
    stream <<"Items in my bag, item by item: "<< endl;
    
    for(auto& item: my_bag)
    {
        handle_my_bag.handle(item);
    }

    stream << endl;
}

void test_container_of_variants_further_simplified()
{
    using coin_t = int;
    using money_t = double;
    using title_t = const char*;

    using bag_t = types::vector_of_variants_t<coin_t, money_t, title_t>;
    using item_t = bag_t::variant_type;

    bag_t my_bag; // vector of variants

    my_bag.emplace_back(10);
    my_bag.emplace_back(100.23);
    my_bag.emplace_back("Modern C++ Programming");
    my_bag.emplace_back("C++17 Standard");

    types::overloads handle_my_bag 
    { 
        [](const coin_t& coin_count)
        {
            stream <<"There are " << coin_count <<" coins."<<nl;
        },

        [](const money_t& money)
        {
            stream <<"I have " << money <<" bucks." << nl;
        },

        [](title_t& title)
        {
            stream <<"I like reading \"" << title << "\"."<<nl;
        }
        
    }; // constructor of types::overloads
   
    stream <<"Items in my bag, item by item, in in-order: "<< endl;
    
    handle_my_bag.for_each(my_bag);
    
    stream << endl;

    stream <<"Items in my bag, item by item, in reverse-order: "<< endl;
    
    handle_my_bag.for_each_reverse(my_bag);
    
    stream << endl;
}

void test_container_of_tuples()
{
    using age_t = int;
    using weight_t = double;
    using name_t = const char*;

    using people_t = types::deque_of_tuples_t<age_t, weight_t, name_t>;
    using person_t = people_t::tuple_type;

    people_t friends;

    friends.emplace_back(18, 56.5, "Alice Kim");
    friends.emplace_back(17, 70.6, "Seven Park");
    friends.emplace_back(16, 45.7, "Michelle Lee");

    stream <<"My friends: " << friends << endl;

    stream <<"\nEach of my friends: " << endl;
    for(auto& f: friends)
    {
        stream << f << endl;
    }

    stream << endl;

    stream <<"\nEach properties of my individual friend: " << endl;
    for(auto& f: friends)
    {
        auto [age, weight, name] = f;

        stream << "My name is " << name << endl;
        stream << "I am " << age << " years old."<<endl;
        stream << "I weigh " <<weight << " kg. " << endl << endl;
    }

    stream << endl;
}

int main()
{
    // test_container_of_variants();

    // test_container_of_variants_simplified();

    // test_container_of_variants_further_simplified();

    test_container_of_tuples();
}
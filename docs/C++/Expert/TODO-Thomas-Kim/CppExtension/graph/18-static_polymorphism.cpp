#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

// mixin
template<typename... CallbackTypes>
struct callbacks: public CallbackTypes...
{
    using CallbackTypes::operator()...;
};

// variadic template argument deduction guide
template<typename... CallbackTypes>
callbacks(CallbackTypes...) -> callbacks<CallbackTypes...>;

void test_callbacks()
{
    auto handle_int = [](int value)
    {
        stream <<"Integer value: " << value << endl;
    };

    auto handle_double = [](double value)
    {
        stream << "Double value: " << value << endl;
    };

    auto handle_const_char = [](const char* name)
    {
        stream << "Your name is " << name << endl;
    };

    auto catch_all = [](auto value)
    {
        stream << "Unclassified value: " << value << endl;
    };

    callbacks handle_callbacks{handle_int, 
        handle_double, handle_const_char, catch_all };

    handle_callbacks(1);

    handle_callbacks(22.0/7.0);

    handle_callbacks("Thomas Kim");

    handle_callbacks( std::string("Unclassified") );

}

void test_variant()
{
    using variant_t = std::variant<int, double, const char*>;

    variant_t v;

    auto handle_int = [](int value)
    {
        stream <<"Integer value: " << value << endl;
    };

    auto handle_double = [](double value)
    {
        stream << "Double value: " << value << endl;
    };

    auto handle_const_char = [](const char* name)
    {
        stream << "Your name is " << name << endl;
    };

    auto catch_all = [](auto value)
    {
        stream << "Unclassified value: " << value << endl;
    };

    callbacks handle_callbacks{handle_int, 
        handle_double, handle_const_char, catch_all };

    v = 5;

    auto handle_variant = [&handle_callbacks](auto& v)
    {    
        std::visit(handle_callbacks, v);
    };

    handle_variant(v);

    v = 22.0 / 7.0;

    handle_variant(v);
    
    v = "Thomas Kim";

    handle_variant(v);
}

int main()
{
    // test_callbacks();
    
    test_variant();
}
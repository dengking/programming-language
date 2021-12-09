#include <tpf_output.hpp>

/*
    In this episode, we will learn one of the most important
    and fundamental concepts regarding C++ Template Parameter.

    For example,

    template<typename Type>
    class TemplateClass
    {
        Type m_member; // this m_member can be of ANY TYPE.
    };

    template<typename Type>
    class LvalueTemplateClass
    {
        Type& m_member; // this m_member always ends up with lvalue reference
                        // it can be const lvalue reference or non-const lvalue reference
    };

    template<typename Type>
    class RvalueTemplateClass
    {
        Type&& m_member; // this m_member can be either Lvalue Reference
                         // or Rvalue Reference.
    };

    template<typename Type>
    void  function_with_plain_T(Type arg) // "In Type arg," there is no declarator
    {
        the type of arg can be of ANY TYPE.
        it can be lvalue reference, rvalue reference, or just plain type.

        The type of Type and the type of arg are always the same, identical.
    }

    template<typename Type>
    void function_with_lvalue_reference(Type& arg) // In Type& arg, & is called lvalue reference declarator
    {
        the type of arg is always lvalue reference.

        the type of Type and the type of arg can be different.
        They are always different.
    }

    template<typename Type> // we call Type as template parameter
    void function_with_rvalue_reference(Type&& arg) // we call arg as a function-call parameter
                                                     // In Type&& arg, && is called rvalue reference declarator
    {
        the type of arg can be either lvalue reference, rvalue reference,
        and arg can never be a plain type.

        Don't get confused the type of Type, and the type of arg.

        The type of Type and the type of arg can be the same or different.

        When we test the type name of a Type, we can use

        Tpf_GetTypeName(Type).

        If we want to test the type name of an object (or variable),
        we can use

        Tpf_GetTypeCategory(arg).

        Always test the types of Type or arg using 
        Tpf_GetTypeName() or Tpf_GetTypeCategory()
    }
*/

/*
    When you do template metaprogramming, or using C++ type functions in the #include<type_traits>,
    never trust your intuition. Always test the type functions from C++ Standard Library,
    using Tpf_GetTypeName() and Tpf_GetTypeCategory()
 */
tpf::sstream stream;
auto endl = tpf::endl;

void test_reverse_range_based_for()
{
    // v is lvalue std::vector<int>
    auto v = tpf::make_vector(1, 2, 3, 4, 5, 6, 7, 8, 9);

    // range-based for
    for(auto& e: v)
    {
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: tpf::reverse(v) // C++ STL Container classes
        )
    {
        stream << e << ", ";
    }

    stream << endl;

    // prime numbers less than 20
    // array is a static C++ array
    int array[]{2, 3, 5, 7, 11, 13, 17, 19};

    for(auto& e: array)
    {
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: tpf::reverse(array) // this function handles for static array,
                                     // array is of type int(&)[7]
        )
    {
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: {2, 4, 6, 8, 10})
    {
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: tpf::reverse(2, 4, 6, 8, 10) // this reverse() creates a vector
                                               // this vector is a pure rvalue
        )
    {
        stream << e << ", ";
    }

    stream << endl;
}

template<typename Type>
bool is_parameter_array(Type param)
{
    stream << "The type of Type: " 
        << Tpf_GetTypeName(Type) << endl;

    return std::is_array_v<Type>;
}

template<typename Type>
bool is_array(Type&& param)
{
    stream << "The type of Type: "
        << Tpf_GetTypeName(Type) << endl;

    return std::is_array_v<Type>;
}

template<typename Type>
constexpr bool is_array_type_v = std::is_array_v<std::remove_reference_t<Type>>;

template<typename Type>
bool is_array_fixed(Type&& param)
{
    return is_array_type_v<Type>;
}

void test_is_parameter_array()
{
    int array[]{1, 2, 3};

    stream << "Is array an array? " << is_parameter_array(array) << endl;

    stream << "\nUsing is_array(), is array an array? "
        << is_array(array) << endl;

    stream << "\nUsing is_array(), is array an array? "
        << is_array_fixed(array) << endl;
}

template<typename Type, typename... Types>
void variadic_parametered_function(Type arg, Types... args)
{
    using types_of_Types = tpf::types::type_list_t<Type, Types...>;

    using types_of_function_call_parameters = 
        tpf::types::type_list_t< decltype(arg), decltype(args)...>;

    stream << "types_of_Types: " << types_of_Types{} << endl;

    stream << "types_of_function_call_parameters: "
        << types_of_function_call_parameters{} << endl;
}

template<typename Type, typename... Types>
void variadic_forwarding_reference_parametered_function(Type&& arg, Types&&... args)
{
    using types_of_Types = tpf::types::type_list_t<Type, Types...>;

    using types_of_function_call_parameters = 
        tpf::types::type_list_t< decltype(arg), decltype(args)...>;

    stream << "types_of_Types: " << types_of_Types{} << endl;

    stream << "types_of_function_call_parameters: "
        << types_of_function_call_parameters{} << endl;
}

void test_variadic_parameters()
{
    variadic_parametered_function(1, 2.0f, 3.14);

    stream << endl;

    variadic_parametered_function("String", L"Wide string", 2.0f, 3.14);
}

void test_variadic_forwarding_reference_parameters()
{
    int a = 1;
    float f = 3.0f;

    variadic_forwarding_reference_parametered_function(1, a, 2.0f, f, 3.14);

    stream << endl;

    variadic_forwarding_reference_parametered_function("String", L"Wide string", 1, a, 2.0f, f, 3.14);
}

int main()
{
    // test_reverse_range_based_for();

    // test_is_parameter_array();

    // test_variadic_parameters();

    test_variadic_forwarding_reference_parameters();
}
#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;


void test_const_expression()
{
    const int size = 10;

    // In C/C++, the element count of a static array 
    // should be determined or evaluated at compile-time.
    // According to C++ Standard, size is a constant expression,
    // so this code works.
    int array[size];
}

constexpr size_t get_size()
{
    return 10;
}

void why_constant_expression_is_misnomer()
{
    // is size a constant expression ?
    // no, it is not.
    const int size = get_size();

    /*
    Even though size is specified with keyword const,
    get_size() is executed at run-time.
    At compile-time, the value of size is unknown.

    The present can affect the Future, but the present cannot know
    the value of the Future.

    The compile-time is present, the run-time is future.

    The value of size should be determined or evaluated at compile-time
    to reserve the memory for array at compile-time.

    Even though size is specified with the keyword const,
    but still its value is determined or evaluated at run-time.

    Is size a constant expression? Yes or No.

    Since the value of size does not change, it is a constant expression.
    Since the value of size is not evaluated at compile-time, 
    it is not a contant expression according to C++ Standard.

    If we say it as "compile-time expression," there can be no confusion.
    */

    int array[size];
}

// this is a global variable.
// The address of any global object is evaluated or
// determined at compile-time.
// Global and Static local variables are stored
// in the Data section of the generated binary code.
// Memory for objects in Data section are determined
// or allocated at compile-time.
int gGlobalInt = 10;

template<int* ptr> // ptr is non-type template parameter.
int get_a_value()
{
    return *ptr;
}

void test_non_type_template_parameters()
{
    stream << "The Address of function: " << get_a_value<&gGlobalInt> << endl;

    stream << "The returned value of the function: " << get_a_value<&gGlobalInt>() << endl;
}

template<auto ptr>
auto get_a_value_using_auto()
{
    return *ptr;
}

void test_get_a_value_auto()
{
    // the address of d is determined at compile-time
    // because d is specified with the keyword static.
    static double d;

    // this code is executed at run-time.
    d = 22.0/7.0;

    stream << "The value of d = " << get_a_value_using_auto<&d>() << endl;
}

void test_failure_case()
{
    // the address of d, the local variable residing in the stack,
    // is not determined or evaluated at compile-time,
    // we can know the address of a local stack variable only at 
    // run-time.
    double d = 22.0/7.0;

    stream << "The value of d = " 
         // << get_a_value_using_auto< &d >() 
                            // we cannot pass non-compile-time expression
        << endl;                          // as non-type template arguments.
}

void important_concept_for_compile_time_expression()
{
    // In C/C++, the enumerators of an enum are compile-time expressions.

    using tuple_t = std::tuple<int, double, const char*>;
    enum: size_t{age, weight, name};

    tuple_t person{ 20, 60.0, "Thomas Kim"};

    stream << "I am " << std::get<name>(person) 
        << ". I am " << std::get<age>(person)
        << " years old. "
        << "I weigh " << std::get<weight>(person) << " kg." << endl;

    size_t index = 0;

    stream // << std::get<index> (person) // this code does not work.
                                        // because the value of index is determined
                                        // or evaluated at run-time.
                                        // index is not a compile-time expression.
        << endl;
}

void why_it_fails()
{
    using variant_t = std::variant<int, double, const char*>;

    variant_t item;

    item = 5;

    stream << std::get<0>(item) << endl;

    item = "Thomas Kim";

   // stream << std::get<2>(item) << endl;

    // the value of index is evaluated at run-time.
    auto index = item.index();

    // non-type template argument should be compile-time expression
    stream // << std::get<index> (item) // this code does not work
        << endl;

    // At this point, you should wonder how I implemented
    // stream class such that it can figure the type of item
    // at run-time?
    // The answer is Template Metaprogramming technique.
   stream << item << endl;
}

int main()
{
    // test_const_expression();
    
    //why_constant_expression_is_misnomer();

    // test_non_type_template_parameters();

    // test_get_a_value_auto();

    // test_failure_case();

   //  important_concept_for_compile_time_expression();

    why_it_fails();
}
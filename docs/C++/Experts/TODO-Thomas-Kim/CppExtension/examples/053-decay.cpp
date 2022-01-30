#include <tpf_output.hpp>

template<typename Type>
using flat_type_t1 = std::remove_cv_t<std::remove_reference_t<std::decay_t<Type>>>;

template<typename Type>
using flat_type_t2 = std::remove_cv_t<std::decay_t<std::remove_reference_t<Type>>>;

tpf::sstream  stream;
auto endl = tpf::endl;

void test_flat_type_t()
{
    const int array[]{10};

    using type_of_array = decltype(array);
    using ref_type_of_array = decltype((array));

    stream << Tpf_GetTypeName( type_of_array) << endl;
    stream << Tpf_GetTypeName( ref_type_of_array) << endl;
    stream << Tpf_GetTypeName( flat_type_t1<type_of_array> ) << endl; 
    stream << Tpf_GetTypeName( flat_type_t1<ref_type_of_array> ) << endl;   
    stream << Tpf_GetTypeName( flat_type_t2<type_of_array> ) << endl; 
    stream << Tpf_GetTypeName( flat_type_t2<ref_type_of_array> ) << endl;  
}

int sum(int, float) { return 0; }

template<typename Type>
decltype(auto) decay(Type&& arg)
{
    using type = std::remove_reference_t<Type>;
   
    if constexpr(std::is_array_v<type> || std::is_function_v<type>)
        return std::decay_t<type>(std::forward<Type>(arg));
    else 
        return std::forward<Type>(arg);
}

void test_decay()
{
    const int const_array[]{1, 2, 3, 4};

    int array[] {1, 2, 3};

    decay(const_array);
    std::cout << "\n---------" << std::endl;
    decay(array);

    std::cout << "\n============" << std::endl;

    decay(std::move(const_array));
     std::cout << "\n---------" << std::endl;
    decay(std::move(array));

}

void test_decay_func()
{
    decay(sum);
    
}
int main()
{
    // test_flat_type_t();
    // test_decay();

    test_decay_func();
}
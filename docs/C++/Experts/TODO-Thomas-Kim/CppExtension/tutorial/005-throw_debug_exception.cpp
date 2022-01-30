#include <tpf_output.hpp>

template<typename Type, typename...Types>
auto divide_sequence(Type&& first, Types&&... args)
{
    auto sequence = std::vector<tpf::remove_cv_ref_t<Type>>
        { std::forward<Type>(first), std::forward<Types>(args)... };

    size_t size = sequence.size();
    auto result = sequence.front();

    for(size_t i = 1; i < size; ++i)
    {
        if( sequence[i] == 0)
        {
            tpf::sstream stream;
            stream << "Division By Zero: " << i <<"-th element is zero";
            Tpf_ThrowDebugException(stream);
        }

        result /= sequence[i];
    }

    return result;  
}

int main()
{
    try
    {
        // ((2*3*5)/2)/3
        auto rlt = divide_sequence(2*3*5, 2, 3);
        tpf::sstream stream;
        stream << "((2*3*5)/2)/3 = " << rlt << tpf::endl;

        stream << "((2*3*5)/0)/3 = " << divide_sequence(2*3*5, 0, 3)
            << tpf::endl;
    }
    catch(std::exception& e)
    {
        tpf::console cout;
        cout << e.what() << tpf::endl;
    }

}
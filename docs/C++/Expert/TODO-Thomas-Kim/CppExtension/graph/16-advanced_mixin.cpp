#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

struct Dog
{
    std::string m_name;

    void operator()() const
    {
        stream <<"I am a dog. My name is " << m_name << endl;
    }
};

struct Cat
{
    std::string m_name;

    void operator()() const
    {
        stream <<"I am a cat. My name is " << m_name << endl;
    }
};

struct Cow
{
    std::string m_name;
    double m_weight;

    void operator()()const
    {
        stream <<"I am a cow. My name is " << m_name <<", I weigh "
        << m_weight << " kg." << endl;
    }
};

template<typename... Animals>
struct AnimalFarm: public Animals... // mixin
{
    using Animals::operator()...;

    using animal_types = tpf::types::type_list_t<Animals...>;

    AnimalFarm(Animals... animals): Animals{ std::move(animals) } ... { }

    template<size_t type_index>
    void report()
    {
        // we select type_index-th base class type
        using base_type = 
            tpf::types::select_nth_type_t<type_index, animal_types>;

        // we are calling base type's function-call operator
        base_type::operator()();
    }

    template<size_t StartIndex, size_t EndIndex>
    struct static_loop
    {
        static void report(const AnimalFarm& af)
        {
            if constexpr(StartIndex < EndIndex)
            {
                using base_type = 
                    tpf::types::select_nth_type_t<StartIndex, AnimalFarm::animal_types>;

                auto& base = static_cast<const base_type&>(af);

                base.operator()();
            }

            if constexpr(StartIndex + 1 < EndIndex)
            {
                static_loop<StartIndex + 1, EndIndex>::report(af);
            }
        }

    };

    void operator()() const
    {
        static_loop<0, tpf::types::type_count_v<Animals...> >::report(*this);
    }
};

// Class Template Argument Deduction Guide
template<typename... Animals>
AnimalFarm(Animals...) -> AnimalFarm<Animals...>;

enum animal: size_t {first_animal, second_animal, third_animal };

void test_animal_farm()
{
    Cat cat{"Thomas the Cat"};

    AnimalFarm myfarm { cat, Dog{"Dog the Friendly"}, Cow{"Pretty Cow", 500.0} };

    myfarm.report<animal::first_animal>();
    myfarm.report<animal::second_animal>();
    myfarm.report<animal::third_animal>(); 

    stream << "List all animals " << endl;

    myfarm();
}

int main()
{
    test_animal_farm();
}
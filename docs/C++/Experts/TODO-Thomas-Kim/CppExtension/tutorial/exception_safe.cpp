#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

namespace permu
{

    auto nl = "\n";

    auto& cout = std::cout;

    template<typename ElementType>
    using set_t = std::vector<ElementType>; // container for set elements

    template<typename ElementType>
    using set_of_sets_t = set_t<set_t<ElementType>>; // container for permutations


    template<typename ElementType>
    void enum_permu(set_of_sets_t<ElementType>& permutations, 
        const set_t<ElementType>& L, set_t<ElementType>& R)
    {
        if(L.empty())
        {
            permutations.emplace_back(R);
        }
        else
        {
            size_t size = L.size();
            
            // make a room for last element
            R.emplace_back(ElementType{});

            /*
                if an exception is thrown 
                before R.pop_back() is reached
                this program does not work properly

                we need to handle this issue
                in EXCEPTION-SAFE manner
            */

            auto CreateCleanup = [] (auto& obj)
            {
                struct CleanUpPlease
                {
                    using obj_t = std::remove_reference_t<decltype(obj)>;

                    obj_t& m_obj;

                    CleanUpPlease(obj_t& o): m_obj{o} { }

                    ~CleanUpPlease()
                        { 
                           //  cout <<"Wow... popping back! " << nl;
                            m_obj.pop_back(); 
                        }

                };

                return CleanUpPlease{ obj };
            };

            // clean_block is a local instance of struct CleanUpPlease
            // auto clean_block = CreateCleanup(R);


            auto r_pop_back = [&R](auto ptr) 
            {
                cout <<"Wow, popping again "<< nl;
                R.pop_back();
            };

            using r_type_t = std::remove_reference_t<decltype(R)>;

            // clean_up is also a local variable
            std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                clean_up(&R, r_pop_back);

            for(size_t i_th = 0; i_th < size; ++i_th)
            {
                // copy L to copied_L
                auto copied_L = L;

                // remove i_th element from copied_L
                copied_L.erase(copied_L.begin()+i_th);

                // move i_th element from L to R
                // R.emplace_back( L[i_th] );
                R.back() = L[i_th];

                enum_permu(permutations,copied_L, R);

                // R.pop_back();
            }

            // ONE STAGE is done
            // R.pop_back();
        }  
    }

    template<typename ElementType>
    void test_enum_permutations()
    {
        set_of_sets_t<ElementType> permutations;
        
        set_t<ElementType> L {0, 1, 2, 3, 4};

        set_t<ElementType> R; R.reserve(L.size());

        enum_permu(permutations, L, R);

        for(size_t i = 0; i < permutations.size(); ++i)
        {
            for(size_t j= 0; j < permutations[i].size(); ++j)
            {
                cout << permutations[i][j] <<", ";
            }

            cout << nl;
        }

        cout << nl;
    }
}

int main()
{
    permu::test_enum_permutations<int>();
}

		

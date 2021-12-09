/*
    Author: Thomas Kim
    First Edit: Feb. 04, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o output.exe
    g++ -std=c++17 filename.cpp -ltbb -o output.exe
    cl /EHsc /std:c++17 filename.cpp /Fe: output.exe
    dpcpp -Qstd=c++17 filename.cpp -o output.exe
*/

#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void test_workhorse_basic()
{
    stream <<"\n--- for_workhorse(), Powerful Compile-Time Loop " << endL;

    stream <<" 1. for_workhorse<10>(lambda) - for_workhorse<End>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<10>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );
    
    stream <<" <- [ 0, 10 ), Span: +1, 10 not included" << endL;

    stream <<" 2. for_workhorse<10, 20>(lambda) - for_workhorse<Start, End>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<10, 20>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 10, 20 ), Span: +1, 20 not included" << endL;

    stream <<" 3. for_workhorse<0, 20, 2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<0, 20, 2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 0, 20 ), Span: +2, 20 not included" << endL;

    stream <<" 4. for_workhorse<1, 20, 2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<1, 20, 2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 1, 20 ), Span: +2, 20 not included" << endL;

    stream <<" 5. for_workhorse<20, 0, -2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<20, 0, -2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 20, 0 ), Span: -2, 0 not included" <<endL;

    stream <<" 6. for_workhorse<19, 0, -2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<19, 0, -2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 19, 0 ), Span: -2, 0 not included" <<endL;

    stream <<" 7. for_workhorse<10, 0, -2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<10, 0, -2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 10, 0 ), Span: -2, 0 not included" <<endL;

    stream <<" 8. for_workhorse<9, 0, -2>(lambda) - for_workhorse<Start, End, Increment>(lambda)" << endL;
    stream <<"\t";

    types::for_workhorse<9, 0, -2>(
        
        [](auto i)
        {
            stream << i.Index <<", ";
        }
    );

    stream <<" <- [ 9, 0 ), Span: -2, 0 not included" <<endL;

}

void test_workhorse_multi_level_loop()
{
    stream <<"--- Iterating 2D Loop using for_workhorse() - 4 x 3 tuple matrix " << endL;

    std::tuple M{ 0, 1, 2,  // Row 1
                  3, 4, 5,  // Row 2
                  6, 7, 8,  // Row 3
                  9, 10, 11 /* Row 4 */ }; // 4 x 3 matrix

    types::indexer_2d_t<4, 3> indexer;
  
    stream <<"\t{ ";

    types::for_workhorse<indexer.Row>( [&M, &indexer](auto i, auto i_info)
    {
        stream <<"{ ";

        types::for_workhorse<indexer.Column>([&](auto j, auto j_info)
        {
            // useful for clang++ and GNU g++
            // constexpr auto index = indexer(i, j);
     
            // for compatibility with Microsoft and oneAPI DPC++
            constexpr auto index = indexer.Column * i.Index + j.Index;

            stream << std::get<index>(M);

            if constexpr(j.Index != j_info.Last)
                stream <<", ";
        });

        stream <<" }";

        if constexpr(i.Index != i_info.Last)
            stream <<", ";
    });
    
    stream << " }" << endL;
}

void test_workhorse_print_tuple()
{
    stream <<"--- Printing tuple using for_workhorse() - IN ORDER" << endL;

    std::tuple tuple{ 0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 };

    constexpr auto Size = types::tuple_size_v<decltype(tuple)>;

    types::indexer_t<Size> dimension;
  
    stream <<"\t{ ";

    types::for_workhorse<dimension.Size>( [&tuple, &dimension](auto i, auto i_info)
    {
        stream << std::get<i.Index>(tuple);

        if constexpr(i.Index != i_info.Last)
            stream << ", ";
    });

    stream << " }" << endL;

    stream <<"--- Printing tuple using for_workhorse() - IN REVERSE ORDER" << endL;

    stream <<"\t{ ";

    types::for_workhorse<dimension.Size-1, -1, -1>( [&tuple, &dimension](auto i, auto i_info)
    {
        stream << std::get<i.Index>(tuple);

        if constexpr(i.Index != i_info.Last)
            stream << ", ";
    });

    stream << " }" << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}



int main()
{
    test_workhorse_basic();

    test_workhorse_multi_level_loop();

    test_workhorse_print_tuple();   
}
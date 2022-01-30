/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

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

void test_make_sequence()
{
    stream <<"\n--- Generating sequence is the very first step in C++ Compile-Time Programming!"<< endL;

    stream << "--- make_sequence_t<InOrderSequence, 10>" << endl;
    using seq_t1 = types::make_sequence_t<types::InOrderSequence, 10>;
    stream << seq_t1{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 0, 20>" << endl;
    using seq_t2 = types::make_sequence_t<types::InOrderSequence, 0, 20>;
    stream << seq_t2{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 0, 20, 2>" << endl;
    using seq_t3 = types::make_sequence_t<types::InOrderSequence, 0, 20, 2>;
    stream << seq_t3{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 20, 0, -1>" << endl;
    using seq_t4 = types::make_sequence_t<types::InOrderSequence, 20, 0, -1>;
    stream << seq_t4{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 19, 0, -1>" << endl;
    using seq_t5 = types::make_sequence_t<types::InOrderSequence, 19, 0, -1>;
    stream << seq_t5{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 20, -1, -2>" << endl;
    using seq_t6 = types::make_sequence_t<types::InOrderSequence, 20, -1, -2>;
    stream << seq_t6{} << endL;

    stream << "--- make_sequence_t<InOrderSequence, 19, -1, -2>" << endl;
    using seq_t7 = types::make_sequence_t<types::InOrderSequence, 19, -1, -2>;
    stream << seq_t7{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 10>" << endl;
    using seq_t8 = types::make_sequence_t<types::ReverseSequence, 10>;
    stream << seq_t8{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 0, 20>" << endl;
    using seq_t9 = types::make_sequence_t<types::ReverseSequence, 0, 20>;
    stream << seq_t9{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 0, 20, 2>" << endl;
    using seq_t10 = types::make_sequence_t<types::ReverseSequence, 0, 20, 2>;
    stream << seq_t10{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 20, 0, -1>" << endl;
    using seq_t11 = types::make_sequence_t<types::ReverseSequence, 20, 0, -1>;
    stream << seq_t11{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 19, 0, -1>" << endl;
    using seq_t12 = types::make_sequence_t<types::ReverseSequence, 19, 0, -1>;
    stream << seq_t12{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 20, -1, -2>" << endl;
    using seq_t13 = types::make_sequence_t<types::ReverseSequence, 20, -1, -2>;
    stream << seq_t13{} << endL;

    stream << "--- make_sequence_t<ReverseSequence, 19, -1, -2>" << endl;
    using seq_t14 = types::make_sequence_t<types::ReverseSequence, 19, -1, -2>;
    stream << seq_t14{} << endL;
}

void test_sequence_information()
{
    stream <<"--- Retrieving information of the existing sequence is also important!!" << endL;

    stream <<" sequence = make_sequence_t<types::InOrderSequence, -10, 10, 1>"<<endl;
    using sequence = types::make_sequence_t<types::InOrderSequence, -10, 10, 1>;
    stream << " sequence = " << sequence{} << endL;
    
    stream <<" --- sequence_first_element_v<sequence>"<<endl;
    constexpr auto seq_1_first = types::sequence_first_element_v<sequence>;
    stream <<" First Element: " << seq_1_first << endL;

    stream <<" --- sequence_last_element_v<sequence>"<<endl;
    constexpr auto seq_1_last = types::sequence_last_element_v<sequence>;
    stream <<" Last Element: " << seq_1_last << endL;

    stream <<" --- sequence_element_count_v<sequence>"<<endl;
    constexpr auto seq_1_count = types::sequence_element_count_v<sequence>;
    stream <<" Count of Elements: " << seq_1_count << endL;

    stream <<" --- sequence_nth_element_v<0, sequence>"<<endl;
    constexpr auto seq_1_0th_element = types::sequence_nth_element_v<0, sequence>;
    stream <<" 0-th Element: " << seq_1_0th_element << endL;

    stream <<" --- types::sequence_nth_element_v<5, sequence>"<<endl;
    constexpr auto seq_1_5th_element = types::sequence_nth_element_v<5, sequence>;
    stream <<" 5-th Element: " << seq_1_5th_element << endL;

    stream <<" --- types::sequence_nth_element_v<18, sequence>"<<endl;
    constexpr auto seq_1_18th_element = types::sequence_nth_element_v<18, sequence>;
    stream <<" 18-th Element: " << seq_1_18th_element << endL;

    stream <<" --- sequence_span_v<sequence>"<<endl;
    constexpr auto seq_1_span = types::sequence_span_v<sequence>;
    stream <<" Span (Distance Between 2 Elements): " << seq_1_span << endL;

    stream <<" By using reversed_sequence = reverse_sequence_t<sequence>" << endl;
    stream <<" the original sequence: " << sequence{} <<endl;
    using reversed_sequence = types::reverse_sequence_t<sequence>;
    stream <<" is now reversed as show: " << reversed_sequence{} <<endL;
    
    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}

int main()
{
    test_make_sequence();

    test_sequence_information();
}
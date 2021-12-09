/**
 * @file 002-conversion.cpp
 * @author your name (you@domain.com)
 * @brief Demonstrate How to Implement type_to_string() function
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <tpf_conversion.hpp>
#include <tpf_output.hpp>
#include <vector>

/**
 * @brief String encoding conversion examples
 * 
 * [examples_string_conversion_snippet]
 */
void examples_string_conversion()
{
    using namespace tpf::conversion;

    // load default system locale
    load_default_locale();

    // converts wchar_t to char
    std::cout << "From UTF16 wchar_t to UTF8 char: " 
        << utf16_to_utf8(L'A') << std::endl;

    // converts char to wchar_t
    std::wcout << L"From UTF8 char to UTF16 wchar_t: " 
        << utf8_to_utf16('A') << std::endl;

    // converts UTF16 wchar_t* to std::string
    std::cout  << "From UTF16 wchar_t* to UTF8 std::string: [" 
                << utf16_to_utf8(L"Korean") << "]"<< std::endl;

    // converts UTF8 char to UTF16 std::wstring
    std::wcout << L"From UTF8 char* to UTF16 std::wstring: [" 
                << utf8_to_utf16("char literal string") << L"]"<< std::endl;

    std::string utf8str("I don't know");

    // converts UTF8 std::string to UTF16 std::wstring
    std::wcout << L"From UTF8 std::string to UTF16 std::wstring ["
        << utf8_to_utf16(utf8str) << L"]" <<std::endl;

    std::wstring utf16str(L"UTF16 string");

    // converts UTF16 std::wstring to UTF8 std::string
    std::cout << "From UTF16 std::wstring to UTF8 std::string ["
           << utf16_to_utf8(utf16str) << "]" <<std::endl;
}

/** [examples_string_conversion_snippet] */


void test_output()
{
    using namespace tpf::conversion;

    // load default system locale
    load_default_locale();

    tpf::ostream stream;

    stream.std() << std::boolalpha;

    stream << L"김창희" << " 사랑해요... " << true ; 

    using typelist_t = tpf::types::type_list_t<int, double>;

    auto nl = "\n";

    stream << " - types: "<< typelist_t{} << nl;

}

void test_string_output()
{
    using namespace tpf::conversion;

    // load default system locale
    load_default_locale();
    auto nl = "\n";

    tpf::ostream stream;

    stream << std::string("김창희");

    stream << std::wstring(L" 뭔데요?") << nl;
}

void test_container_output()
{
    using namespace tpf::conversion;
    auto nl = "\n";

    // load default system locale
    load_default_locale();

    tpf::ostream stream;
    
    std::vector<int> v{1, 2, 3, 4, 5};

    stream << v << " - 사랑해요, 김창희..." << nl;

}

int main()
{
   test_output();
    // test_string_output();
    // test_container_output();
   
}
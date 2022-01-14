
struct MyStruct
{
    char *str1;
    char *str2;
};
MyStruct *foo();

// g++ --std=c++11 -fpic -shared example.cpp -o libexample.so
// swig -c++ -python  example.i
// g++ --std=c++11 -fpic -shared -L. -I. -I/usr/include/python3.8/ example_wrap.cxx -o _example.so -lexample 
// -lexample 一定要放到最后，否则会导致错误
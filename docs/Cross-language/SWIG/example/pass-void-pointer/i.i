// SWIG interface
%module example
%{
#include "test.h"
%}
%include "carrays.i"
%include "cdata.i"
%array_class(int, intArray);


// swig -c++ -python -threads -o example.cc i.i
// g++ --std=c++11 -fpic -shared  -I/usr/include/python3.9/ example.cc test.cc  -o _example.so  -g

%include "test.h"
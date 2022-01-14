%module example

%{
#include "example.h"
#include <iostream>
%}
// Tell SWIG to wrap foo "properly", but that you don't know anything about it:
struct MyStruct;

%include "example.h"

%newobject foo;
%nodefaultctor MyStruct;
%extend MyStruct {
    ~MyStruct() {
       free($self->str1);
       free($self->str2);
       std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
}


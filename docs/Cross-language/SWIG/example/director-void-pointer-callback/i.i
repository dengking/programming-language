%module example
%{
#include "test.h"
%}

%feature("director") Interface;

%include "test.h"

// swig -c++ -python  -o wrap.cc i.i
// g++ --std=c++11 -fpic -shared -L/home/dengkai/shared/g2-python/linux/lib/x86_64/p1 -L/usr/lib/x86_64-linux-gnu -I. -I/usr/include/python3.9/ wrap.cc -o _example.so -g
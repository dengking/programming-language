#!/bin/bash

swig -c++ -python -o example_wrap.cc interface.i

clang++ --std=c++11 -fpic -shared -I/Users/dengkai01/opt/anaconda3/pkgs/python-3.9.7-h88f2d9e_1/include/python3.9/ example_wrap.cc example.cc -o _example.so -g

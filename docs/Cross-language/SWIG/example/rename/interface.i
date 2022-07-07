%module example
%{
#include "example.h"
%}

%rename(my_print) print;

%include "example.h"
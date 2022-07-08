/* File : example.i */
%module(directors="1") example
%{
#include "example.h"
%}

%include "std_function.i"

%include "example.h"

%std_function(SwigCallback, void, const ParamStruct &);
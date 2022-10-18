# 13 Typemaps

## 13.1 Introduction 

That said, let's start with a short disclaimer that "typemaps" are an advanced customization feature that provide direct access to SWIG's low-level **code generator**. Not only that, they are an integral part of the **SWIG C++ type system** (a non-trivial topic of its own). Typemaps are generally *not* a required part of using SWIG. Therefore, you might want to re-read the earlier chapters if you have found your way to this chapter with only a vague(模糊) idea of what SWIG already does by default. 



### 13.1.1 Type conversion

> NOTE: 
>
> 一、本节描述的type convention指的是type在源语言和目标语言之间的转换

One of the most important problems in wrapper code generation is the conversion or marshalling of datatypes between programming languages. Specifically, for every C/C++ declaration, SWIG must somehow generate wrapper code that allows values to be passed back and forth between languages. Since every programming language represents data differently, this is not a simple of matter of simply linking code together with the C linker. Instead, SWIG has to know something about how data is represented in each language and how it can be manipulated. 

To illustrate, suppose you had a simple C function like this:

```c++
int factorial(int n);
```

To access this function from Python, a pair of Python API functions are used to convert integer values. For example: 

```c++
long PyInt_AsLong(PyObject *obj); /* Python --> C */
PyObject *PyInt_FromLong(long x); /* C --> Python */
```



```c++

PyObject *wrap_factorial(PyObject *self, PyObject *args)
{
  int arg1;
  int result;
  PyObject *obj1;
  PyObject *resultobj;
  if (!PyArg_ParseTuple("O:factorial", &obj1))
    return NULL;
  arg1 = PyInt_AsLong(obj1);
  result = factorial(arg1);
  resultobj = PyInt_FromLong(result);
  return resultobj;
}
```



### 13.1.2 Typemaps

Since type handling is so central to wrapper code generation, SWIG allows it to be completely defined (or redefined) by the user. To do this, a special **%typemap** directive is used. 

```c++
/* Convert from Python --> C */ 
% typemap(in) int { 
  $1 = PyInt_AsLong($input); 
} 

/* Convert from C --> Python */
% typemap(out) int { 
  $result = PyInt_FromLong($1); 
}
```

Note that this isn't always the case as some target language modules allow target language code within the typemaps which gets generated into target language specific files. Within this code, a number of special variables prefixed with a `$` are expanded. These are really just placeholders for C/C++ variables that are generated in the course of creating the wrapper function. In this case, **$input** refers to an input object that needs to be converted to C/C++ and **$result** refers to an object that is going to be returned by a wrapper function. **$1** refers to a C/C++ variable that has the same type as specified in the typemap declaration (an **int** in this example). 



### 13.1.3 Pattern matching



## examples

stackoverflow [SWIG: How to map an enum to integer without %including the header?](https://stackoverflow.com/questions/61917132/swig-how-to-map-an-enum-to-integer-without-including-the-header)

stackoverflow [Create SWIG C# wrapper for function that contains void* parameter](https://stackoverflow.com/questions/25310530/create-swig-c-sharp-wrapper-for-function-that-contains-void-parameter)
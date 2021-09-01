# `libclang`



https://llvm.org/devmtg/2010-11/Gregor-libclang.pdf

https://sudonull.com/post/907-An-example-of-parsing-C-code-using-libclang-in-Python



## Example

### stackoverflow [python-clang: Getting Template Arguments](https://stackoverflow.com/questions/41333185/python-clang-getting-template-arguments)



```c++
template <typename T>
class X {};
X<bool> x;
```

I'd like to be able to figure out that `X` is instantiated with a `bool` as its template parameter.

First, it seems that some functions, such as `get_num_template_arguments`, are not exposed via `python-clang` in the first place, which is where [cymbal](https://pypi.python.org/pypi/cymbal/1.0.0) seems to come into play to monkey-patch `python-clang`.

With this, I was able to get this far:

```Python
#!/usr/bin/env python

import clang.cindex

clang.cindex.Config.set_library_file('/usr/lib/x86_64-linux-gnu/libclang-3.9.so.1')

index = clang.cindex.Index.create()

source = """
template <typename T> class X {};
X<bool> x;
"""

######### stolen from cymbal

from ctypes import c_uint, c_int

def find_libclang_function(function):
    return getattr(clang.cindex.conf.lib, function)

def monkeypatch_helper(classtype, name, library_function, args, result):
    if hasattr(classtype, name):
        raise ('failed to add method, %s is already available' % name) 
    f = find_libclang_function(library_function)
    f.argtypes = args
    f.restype = result
    def impl(*args):
        return f(*args)
    setattr(classtype, name, impl)

def monkeypatch_type(method_name, library_function, args, result):
    monkeypatch_helper(clang.cindex.Type, method_name, library_function, args, result)



monkeypatch_type('get_template_argument_type',
                        'clang_Type_getTemplateArgumentAsType',
                        [clang.cindex.Type, c_uint],
                        clang.cindex.Type)

monkeypatch_type('get_num_template_arguments',
                        'clang_Type_getNumTemplateArguments',
                        [clang.cindex.Type],
                        c_int)
######### /stolen from cymbal

# helpers for visiting the AST recursively
def visit(node, func):
    func(node)
    for c in node.get_children():
        visit(c, func)

def visit_depth(node, func, depth=0):
    func(node, depth)
    for c in node.get_children():
        visit_depth(c, func, depth+1)

# parse the TU
tu = clang.cindex.TranslationUnit.from_source('t.cpp', ['-std=c++11'], unsaved_files=[('t.cpp', source)])

# show the AST
def astprinter(node, depth):
    print " "*depth, node.kind, node.spelling
visit_depth(tu.cursor, astprinter)    

# find everything with a template and try to extract the template argument
def template_finder(node):
    if hasattr(node, 'type') and node.type.get_num_template_arguments() != -1:
        print node.type.get_num_template_arguments(), node.spelling, node.kind, node.get_template_argument_type(0).kind
visit(tu.cursor, template_finder)
```



#### [A](https://stackoverflow.com/a/41337188)

I think all you're really missing is `.type` in a couple of places in template finder, but for reference, this works for me, even on clang as old as 3.7

```python
import clang.cindex
from clang.cindex import *
import cymbal
from ctypes import *

cymbal.monkeypatch_type('get_template_argument_type',
                        'clang_Type_getTemplateArgumentAsType',
                        [Type, c_uint],
                        Type)

cymbal.monkeypatch_type('get_num_template_arguments',
                        'clang_Type_getNumTemplateArguments',
                        [Type],
                        c_int)


# check if the cursor's type is a template
def is_template(node):
    return hasattr(node, 'type') and node.type.get_num_template_arguments() != -1

index = clang.cindex.Index.create()

source = """
template <typename T> class X {};
X<bool> x;
"""
# parse the TU
tu = clang.cindex.TranslationUnit.from_source('t.cpp', ['-std=c++11'], unsaved_files=[('t.cpp', source)])

for c in tu.cursor.walk_preorder():
    if is_template(c):
        t = c.type
        print t.kind, t.spelling, t.get_num_template_arguments()
        print t.get_template_argument_type(0).spelling
```

Which gives:

```csharp
TypeKind.UNEXPOSED X<bool> 1
bool
TypeKind.UNEXPOSED X<bool> 1
bool
```

> NOTE: 
>
> 完整程序如下:
>
> ```python
> import clang.cindex
> from clang.cindex import *
> import cymbal
> from ctypes import *
> 
> cymbal.monkeypatch_type('get_template_argument_type',
>                         'clang_Type_getTemplateArgumentAsType',
>                         [Type, c_uint],
>                         Type)
> 
> cymbal.monkeypatch_type('get_num_template_arguments',
>                         'clang_Type_getNumTemplateArguments',
>                         [Type],
>                         c_int)
> 
> 
> # check if the cursor's type is a template
> def is_template(node):
>     return hasattr(node, 'type') and node.type.get_num_template_arguments() != -1
> 
> index = clang.cindex.Index.create()
> 
> source = """
> template <typename T> class X {};
> X<bool> x;
> """
> # parse the TU
> tu = clang.cindex.TranslationUnit.from_source('t.cpp', ['-std=c++11'], unsaved_files=[('t.cpp', source)])
> 
> for c in tu.cursor.walk_preorder():
>     if is_template(c):
>         t = c.type
>         print(t.kind, t.spelling, t.get_num_template_arguments())
>         print(t.get_template_argument_type(0).spelling)
> ```
>
> 



### stackoverflow [can libclang parse the CRTP pattern?](https://stackoverflow.com/questions/42795408/can-libclang-parse-the-crtp-pattern)

I am trying to use libclang for parsing C++, but it seems to have problem with the CRTP pattern, i.e when a class inherits from a template that is instantiated with the derived class:

```C++
// The Curiously Recurring Template Pattern (CRTP)
template<class T>
class Base
{
  // methods within Base can use template to access members of Derived
};

class Derived : public Base<Derived>
{
  // ...
};
```

I want libclang to find the cursor kind `CXCursor_CXXBaseSpecifier`, but it only gives me the `CXCursor_ClassDecl` kind.

If Base was not a template class, libclang will find the `CXCursor_CXXBaseSpecifier`.

What I want to accomplish is to find classes that inherits from Base, but it is not possible when libclang only gives the ClassDecl kind. The 'public Base' does not give a cursor, it seems to be ignored.

Does anyone know how to solve this?

#### [A](https://stackoverflow.com/a/42871503)

Cursors that have a kind of `CXX_BASE_SPECIFIER` will have child cursors which allow you to determine this information. In cases where the base specifier refers to a template class, it will have two child nodes (of kind) TEMPLATE_REF and TYPE_REF. You can use that information in the TEMPLATE_REF node to compare against a class template cursor.

To make this clearer, I'll show a small example. Pretty printing the (libclang) version of the AST of the following:

```cpp
template<class T>
class Base { };
class X1 : public Base<X1> {};
class Y1 {};
class X2 : public Y1 {};
```

Gives:

```cpp
TRANSLATION_UNIT tmp.cpp
  +--CLASS_TEMPLATE Base
  |  +--TEMPLATE_TYPE_PARAMETER T
  +--CLASS_DECL X1
  |  +--CXX_BASE_SPECIFIER Base<class X1>
  |     +--TEMPLATE_REF Base
  |     +--TYPE_REF class X1
  +--CLASS_DECL Y1
  +--CLASS_DECL X2
     +--CXX_BASE_SPECIFIER class Y1
        +--TYPE_REF class Y1
```

So a basic approach is:

1. For each class
2. Find all it's children that have `CXX_BASE_SPECIFIER` kind
3. For the base nodes, find all of those have two children (and one of those is has the kind `TEMPLATE_REF`)
4. For the `TEMPLATE_REF` nodes, check if they have a common definition with the class template of interest.

Given that this would be a very big piece of code in C/C++ (for stackoverflow), I'll present a Python 2 version that implements those steps, that should be fairly easy to translate.

```cpp
import clang
from clang.cindex import CursorKind


def find_template_class(name):
    for c in tu.cursor.walk_preorder():
        if (c.kind == CursorKind.CLASS_TEMPLATE) and (c.spelling == name):
            return c

def inherits_from_template_class(node, base):
    for c in node.get_children():
        if c.kind != CursorKind.CXX_BASE_SPECIFIER:
            continue
        children = list(c.get_children())
        if len(children) != 2:
            continue
        if children[0].kind != CursorKind.TEMPLATE_REF:
            continue
        ctd = children[0].get_definition()
        if ctd == base:
            return True
    return False

idx = clang.cindex.Index.create()
tu = idx.parse('tmp.cpp', unsaved_files=[('tmp.cpp', s)],  args='-xc++'.split())
base = find_template_class('Base')
for c in tu.cursor.walk_preorder():
    if CursorKind.CLASS_DECL != c.kind:
        continue
    if inherits_from_template_class(c, base):
        print c.spelling
```





### stackoverflow [python.clang AST parsing: getting c++ template argument of field declaration](https://stackoverflow.com/questions/59171077/python-clang-ast-parsing-getting-c-template-argument-of-field-declaration)

> NOTE: 
>
> 一、这篇文章描述的场景和我目前的需求是非常贴合的
>
> 二、下面是对这篇文章的中文翻译:
>
> coder [python - python.clang AST解析:获取字段声明的c++模板参数](https://www.coder.work/article/6227893)

I am using the abstract syntax tree that clang provides through the python interface, trying to parse a simple structure containing a `std::vector`:

```cpp
#include <vector>

struct outer_t
{
    std::vector<int> vec_of_ints;
};
```

I would like to get the **template argument** of the vector, but I cannot find a reference to it in the respective node of the AST. The `get_num_template_arguments()` member function returns `-1`. I therefore think that the `get_template_* function`s cannot be used. I tried the following:

```python
import sys
import clang.cindex
clang.cindex.Config.set_library_file("/usr/lib/llvm-6.0/lib/libclang.so.1")

class Walker:
    def __init__(self, filename):
        self.filename = filename

    def walk(self, node):
        node_in_file =  bool(node.location.file and node.location.file.name == self.filename)
        if node_in_file:
            print(f"node.spelling = {node.spelling:14}, node.kind = {node.kind}")
            if node.kind == clang.cindex.CursorKind.TEMPLATE_REF:
                print(f"node.get_num_template_arguments = {node.get_num_template_arguments()}")
        for child in node.get_children():
            self.walk(child)

filename = sys.argv[1]
index = clang.cindex.Index.create()
translation_unit = index.parse(filename)

root = translation_unit.cursor        
walker = Walker(filename)
walker.walk(root)
```

This produces the following result:

```ini
node.spelling = vec_of_ints   , node.kind = CursorKind.FIELD_DECL
node.spelling = std           , node.kind = CursorKind.NAMESPACE_REF
node.spelling = vector        , node.kind = CursorKind.TEMPLATE_REF
node.get_num_template_arguments = -1
```

Is there another way to get the template argument or am I doing something wrong ?

#### [A](https://stackoverflow.com/a/60678855)

The `TEMPLATE_REF` cursor kind doesn't seem to have any information about its arguments, at least for this example; I don't know why. But `FIELD_DECL` has a `type` that can have template arguments. Here is a minimally modified version of your code that prints the number of template arguments for the `vec_of_ints` field in your example code:

```python
import sys
import clang.cindex
clang.cindex.Config.set_library_file("/usr/lib/llvm-6.0/lib/libclang.so.1")

class Walker:
    def __init__(self, filename):
        self.filename = filename

    def walk(self, node):
        node_in_file =  bool(node.location.file and node.location.file.name == self.filename)
        if node_in_file:
            print(f"node.spelling = {node.spelling:14}, node.kind = {node.kind}")
            # -------- BEGIN modified section --------
            type = node.type
            if type is not None:
                ntargs = type.get_num_template_arguments()
                if ntargs > 0:
                    print(f"  type.spelling = {type.spelling}")
                    print(f"  type.get_num_template_arguments = {ntargs}")
            # -------- END modified section --------
        for child in node.get_children():
            self.walk(child)

filename = sys.argv[1]
index = clang.cindex.Index.create()
translation_unit = index.parse(filename)

root = translation_unit.cursor
walker = Walker(filename)
walker.walk(root)
```

When run on your example input file, I get:

```ini
node.spelling = outer_t       , node.kind = CursorKind.STRUCT_DECL
node.spelling = vec_of_ints   , node.kind = CursorKind.FIELD_DECL
  type.spelling = std::vector<int>
  type.get_num_template_arguments = 1
node.spelling = std           , node.kind = CursorKind.NAMESPACE_REF
node.spelling = vector        , node.kind = CursorKind.TEMPLATE_REF
```

I don't claim this will handle all cases of templates appearing in code. I discovered the above through a combination of trial and error and reading the [`clang/cindex.py`](https://github.com/llvm-mirror/clang/blob/master/bindings/python/clang/cindex.py) library module source file. But it hopefully serves as a useful starting point.

In any case, one important thing to understand about the Clang AST (and pretty much all C/C++ ASTs) is that types are not nodes in the main syntax tree. Rather, types are semantic *interpretations* of some nodes in that tree, and thus sort of sit to the side. That's why they don't appear as arguments to `walk`.



### stackoverflow [Discover Class Template through extern template with libclang](https://stackoverflow.com/questions/68199812/discover-class-template-through-extern-template-with-libclang)

Given a file `test.cpp`:

```cpp
#include <string>

struct thing
{
    std::string foo;
};
```

I can parse this with libclang using the following Python:

```python
#!/usr/bin/env python3

from clang import cindex

idx = cindex.Index.create()
tu = idx.parse('test.cpp')

# Sanity check: Print any diagnostics
for diagnostic in tu.diagnostics:
    print(f'DIAGNOSTIC: {diagnostic}')

def get_all_children(node):
    yield node

    for child in node.get_children():
        yield from get_all_children(child)

def find_child(node, predicate):
    for item in get_all_children(node):
        if predicate(item):
            return item

    raise ValueError('Nothing matched predicate')

# Find the thing structure
thing = find_child(tu.cursor, lambda x: x.kind == cindex.CursorKind.STRUCT_DECL and x.spelling == 'thing')

# Get the std::string field
thing_foo = find_child(thing, lambda x: x.kind == cindex.CursorKind.FIELD_DECL and x.spelling == 'foo')

print(thing_foo.type.spelling)
print(thing_foo.type.get_canonical().spelling)
```

This script will print:

```cpp
std::string
std::__cxx11::basic_string<char>
```

All of this expected. However, what I want to get to is the type definition for the `basic_string` class template and, try as I might, I cannot seem to find the correct sequence of calls to get there. The issue is that the "canonical" definition (`thing_foo.type.get_canonical()`) always refers to the `extern template` definition in `bits/basic_string.tcc` (`extern template class basic_string<char>;`). After this, I don't know where to go to get through the declaration to `basic_string<T, Traits>`.

The ultimate goal here is to ask questions of the referred-to type like "Is there a `.begin()` function," so I need the actual class template.

### stackoverflow [Discover Class Template through extern template with libclang](https://stackoverflow.com/questions/68199812/discover-class-template-through-extern-template-with-libclang)



### 罗列所有的成员函数

stackoverflow [How to retrieve class names and its method names from C++ header using python](https://stackoverflow.com/questions/57072048/how-to-retrieve-class-names-and-its-method-names-from-c-header-using-python)



### C pointer to function

stackoverflow [extract argument & return types from C function pointer with libclang](https://stackoverflow.com/questions/47444064/extract-argument-return-types-from-c-function-pointer-with-libclang)



### typedef

stackoverflow [How can I parse a typedef for a function pointer using clang API to get the function pointer name?](https://stackoverflow.com/questions/24054433/how-can-i-parse-a-typedef-for-a-function-pointer-using-clang-api-to-get-the-func)



stackoverflow [Get typedef type and name with libclang?](https://stackoverflow.com/questions/25416548/get-typedef-type-and-name-with-libclang)



在Python libclang中，使用`get_canonical()`来获得



### canonical types in Clang

它和`typedef`有关。

stackoverflow [What are canonical types in Clang?](https://stackoverflow.com/questions/25231080/what-are-canonical-types-in-clang)



### qualified name、namespace



## github [**cindex-dump.py**](https://github.com/llvm-mirror/clang/blob/master/bindings/python/examples/cindex/cindex-dump.py)

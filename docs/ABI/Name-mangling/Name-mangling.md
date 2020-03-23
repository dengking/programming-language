[TOC]

# [Name mangling](https://en.wikipedia.org/wiki/Name_mangling#Name_mangling_in_C++)

In [compiler](https://en.wikipedia.org/wiki/Compiler) construction, **name mangling** (also called **name decoration**) is a technique used to solve various problems caused by the need to resolve unique names for **programming entities** in many modern [programming languages](https://en.wikipedia.org/wiki/Programming_language).

It provides a way of encoding additional information in the name of a [function](https://en.wikipedia.org/wiki/Function_(programming)), [structure](https://en.wikipedia.org/wiki/Structure), [class](https://en.wikipedia.org/wiki/Class_(computer_science)) or another [datatype](https://en.wikipedia.org/wiki/Datatype) in order to pass more semantic information from the [compilers](https://en.wikipedia.org/wiki/Compiler) to [linkers](https://en.wikipedia.org/wiki/Linker_(computing)).

***SUMMARY*** : 上面这段话描述了使用name mangling的意图

The need arises where the language allows different **entities** to be named with the same [identifier](https://en.wikipedia.org/wiki/Identifier) as long as they occupy a different [namespace](https://en.wikipedia.org/wiki/Namespace) (where a namespace is typically defined by a module, class, or explicit *namespace* directive) or have different signatures (such as [function overloading](https://en.wikipedia.org/wiki/Function_overloading)).

Any [object code](https://en.wikipedia.org/wiki/Object_code) produced by compilers is usually linked with other pieces of object code (produced by the same or another compiler) by a type of program called a [linker](https://en.wikipedia.org/wiki/Linker_(computing)). The linker needs a great deal of information on each program entity. For example, to correctly link a function it needs its name, the number of arguments and their types, and so on.

## Examples

### C

Although name mangling is not generally required or used by languages that do not support [function overloading](https://en.wikipedia.org/wiki/Function_overloading) (such as C and classic Pascal), they use it in some cases to provide additional information about a function. For example, compilers targeted at Microsoft Windows platforms support a variety of [calling conventions](https://en.wikipedia.org/wiki/Calling_convention), which determine the manner in which parameters are sent to subroutines and results returned. Because the different **calling conventions** are not compatible with one another, compilers mangle symbols with codes detailing which **convention** should be used to call the specific routine.

***SUMMARY*** : encode calling convention in the name

The **mangling scheme** was established by Microsoft, and has been informally followed by other compilers including Digital Mars, Borland, and GNU GCC, when compiling code for the Windows platforms. The scheme even applies to other languages, such as [Pascal](https://en.wikipedia.org/wiki/Pascal_programming_language), [D](https://en.wikipedia.org/wiki/D_programming_language), [Delphi](https://en.wikipedia.org/wiki/Embarcadero_Delphi), [Fortran](https://en.wikipedia.org/wiki/Fortran), and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)). This allows subroutines written in those languages to call, or be called by, existing Windows libraries using a calling convention different from their default.

When compiling the following C examples:

```c
int _cdecl    f (int x) { return 0; }
int _stdcall  g (int y) { return 0; }
int _fastcall h (int z) { return 0; }
```

32 bit compilers emit, respectively:

```c
_f
_g@4
@h@4
```

In the `stdcall` and `fastcall` mangling schemes, the function is encoded as `_**name**@**X**` and `@**name**@**X**` respectively, where **X** is the number of bytes, in decimal, of the argument(s) in the parameter list (including those passed in registers, for fastcall). In the case of `cdecl`, the function name is merely prefixed by an underscore.

The 64-bit convention on Windows (Microsoft C) has no leading underscore. This difference may in some rare cases lead to unresolved externals when porting such code to 64 bits. For example, Fortran code can use 'alias' to link against a C method by name as follows:

```Fortran 
SUBROUTINE f()
!DEC$ ATTRIBUTES C, ALIAS:'_f' :: f
END SUBROUTINE
```

This will compile and link fine under 32 bits, but generate an unresolved external '_f' under 64 bits. One workaround for this is not to use 'alias' at all (in which the method names typically need to be capitalized in C and Fortran). Another is to use the BIND option:

```Fortran 
SUBROUTINE f() BIND(C,NAME="f")
END SUBROUTINE
```

### C++

[C++](https://en.wikipedia.org/wiki/C%2B%2B) compilers are the most widespread users of **name mangling**. The first C++ compilers were implemented as translators to [C](https://en.wikipedia.org/wiki/C_(programming_language)) source code, which would then be compiled by a C compiler to object code; because of this, symbol names had to conform to C identifier rules. Even later, with the emergence of compilers which produced machine code or assembly directly, the system's [linker](https://en.wikipedia.org/wiki/Linker_(computing)) generally did not support C++ symbols, and mangling was still required.

The [C++](https://en.wikipedia.org/wiki/C%2B%2B) language does not define a standard decoration scheme, so each compiler uses its own. C++ also has complex language features, such as [classes](https://en.wikipedia.org/wiki/C%2B%2B_class), [templates](https://en.wikipedia.org/wiki/C%2B%2B_template), [namespaces](https://en.wikipedia.org/wiki/Namespace_(C%2B%2B)), and [operator overloading](https://en.wikipedia.org/wiki/Operator_overloading), that alter the meaning of specific symbols based on context or usage. Meta-data about these features can be disambiguated(消除) by mangling (decorating) the name of a [symbol](https://en.wikipedia.org/wiki/Debug_symbol). Because the **name-mangling systems** for such features are not standardized across compilers, few linkers can link object code that was produced by different compilers.

#### Simple example[[edit](https://en.wikipedia.org/w/index.php?title=Name_mangling&action=edit&section=4)]

A single C++ translation unit might define two functions named `f()`:

```cpp
int  f (void) { return 1; }
int  f (int)  { return 0; }
void g (void) { int i = f(), j = f(0); }
```

These are distinct functions, with no relation to each other apart from the name. The C++ compiler therefore will encode the type information in the symbol name, the result being something resembling:

```cpp
int  __f_v (void) { return 1; }
int  __f_i (int)  { return 0; }
void __g_v (void) { int i = __f_v(), j = __f_i(0); }
```

Even though its name is unique, `g()` is still mangled: name mangling applies to **all** symbols.

#### Complex example[[edit](https://en.wikipedia.org/w/index.php?title=Name_mangling&action=edit&section=5)]

The mangled symbols in this example, in the comments below the respective identifier name, are those produced by the GNU GCC 3.x compilers:

```cpp
namespace wikipedia 
{
   class article 
   {
   public:
      std::string format (void); 
         /* = _ZN9wikipedia7article6formatEv */

      bool print_to (std::ostream&); 
         /* = _ZN9wikipedia7article8print_toERSo */

      class wikilink 
      {
      public:
         wikilink (std::string const& name);
            /* = _ZN9wikipedia7article8wikilinkC1ERKSs */
      };
   };
}
```

All mangled symbols begin with **_Z** (note that an identifier beginning with an underscore followed by a capital is a [reserved identifier](https://en.wikipedia.org/wiki/Reserved_identifier) in C, so conflict with user identifiers is avoided); for nested names (including both namespaces and classes), this is followed by `N`, then a series of <length, id> pairs (the length being the length of the next identifier), and finally `E`. For example, `wikipedia::article::format` becomes

```c
_ZN9Wikipedia7article6formatE
```

For functions, this is then followed by the type information; as `format()` is a `void` function, this is simply `v`; hence:

```
_ZN9Wikipedia7article6formatEv
```

For `print_to`, the standard type `std::ostream` (which is a `typedef` for `std::basic_ostream<char, std::char_traits<char> >`) is used, which has the special alias `So`; a reference to this type is therefore `RSo`, with the complete name for the function being:

```
_ZN9Wikipedia7article8print_toERSo
```

#### How different compilers mangle the same functions[[edit](https://en.wikipedia.org/w/index.php?title=Name_mangling&action=edit&section=6)]

There isn't a standard scheme by which even trivial C++ identifiers are mangled, and consequently different compilers (or even different versions of the same compiler, or the same compiler on different platforms) mangle public symbols in radically different (and thus totally incompatible) ways. Consider how different C++ compilers mangle the same functions:

| Compiler                                                     | `void h(int)`      | `void h(int, char)`    | `void h(void)`        |
| ------------------------------------------------------------ | ------------------ | ---------------------- | --------------------- |
| Intel C++ 8.0 for Linux                                      | `_Z1hi`            | `_Z1hic`               | `_Z1hv`               |
| HP aC++ A.05.55 IA-64                                        |                    |                        |                       |
| IAR EWARM C++ 5.4 ARM                                        |                    |                        |                       |
| [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) 3.*x* and higher |                    |                        |                       |
| [Clang](https://en.wikipedia.org/wiki/Clang) 1.*x* and higher[[1\]](https://en.wikipedia.org/wiki/Name_mangling#cite_note-1) |                    |                        |                       |
| IAR EWARM C++ 7.4 ARM                                        | `_Z<number>hi`     | `_Z<number>hic`        | `_Z<number>hv`        |
| [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) 2.9*x* | `h__Fi`            | `h__Fic`               | `h__Fv`               |
| HP aC++ A.03.45 PA-RISC                                      |                    |                        |                       |
| [Microsoft Visual C++](https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B) v6-v10 ([mangling details](https://en.wikiversity.org/wiki/Visual_C%2B%2B_name_mangling)) | `?h@@YAXH@Z`       | `?h@@YAXHD@Z`          | `?h@@YAXXZ`           |
| [Digital Mars](https://en.wikipedia.org/wiki/Digital_Mars) C++ |                    |                        |                       |
| Borland C++ v3.1                                             | `@h$qi`            | `@h$qizc`              | `@h$qv`               |
| OpenVMS C++ V6.5 (ARM mode)                                  | `H__XI`            | `H__XIC`               | `H__XV`               |
| OpenVMS C++ V6.5 (ANSI mode)                                 |                    | `CXX$__7H__FIC26CDH77` | `CXX$__7H__FV2CB06E8` |
| OpenVMS C++ X7.1 IA-64                                       | `CXX$_Z1HI2DSQ26A` | `CXX$_Z1HIC2NP3LI4`    | `CXX$_Z1HV0BCA19V`    |
| SunPro CC                                                    | `__1cBh6Fi_v_`     | `__1cBh6Fic_v_`        | `__1cBh6F_v_`         |
| Tru64 C++ V6.5 (ARM mode)                                    | `h__Xi`            | `h__Xic`               | `h__Xv`               |
| Tru64 C++ V6.5 (ANSI mode)                                   | `__7h__Fi`         | `__7h__Fic`            | `__7h__Fv`            |
| Watcom C++ 10.6                                              | `W?h$n(i)v`        | `W?h$n(ia)v`           | `W?h$n()v`            |

Notes:

- The Compaq C++ compiler on OpenVMS VAX and Alpha (but not IA-64) and Tru64 has two name mangling schemes. The original, pre-standard scheme is known as ARM model, and is based on the name mangling described in the C++ Annotated Reference Manual (ARM). With the advent of new features in standard C++, particularly [templates](https://en.wikipedia.org/wiki/Template_(programming)), the ARM scheme became more and more unsuitable — it could not encode certain function types, or produced identical mangled names for different functions. It was therefore replaced by the newer "ANSI" model, which supported all ANSI template features, but was not backwards compatible.
- On IA-64, a standard [Application Binary Interface (ABI)](https://en.wikipedia.org/wiki/Application_binary_interface) exists (see [external links](https://en.wikipedia.org/wiki/Name_mangling#External_links)), which defines (among other things) a standard name-mangling scheme, and which is used by all the IA-64 compilers. GNU GCC 3.*x*, in addition, has adopted the name mangling scheme defined in this standard for use on other, non-Intel platforms.
- The Visual Studio and Windows SDK include the program `undname` which prints the C-style function prototype for a given mangled name.
- On Microsoft Windows, the Intel compiler[[2\]](https://en.wikipedia.org/wiki/Name_mangling#cite_note-2) and [Clang](https://en.wikipedia.org/wiki/Clang)[[3\]](https://en.wikipedia.org/wiki/Name_mangling#cite_note-3) uses the Visual C++ name mangling for compatibility.
- For the IAR EWARM C++ 7.4 ARM compiler the best way to determine the name of a function is to compile with the assembler output turned on and to look at the output in the ".s" file thus generated.

#### Handling of C symbols when linking from C++

The job of the common C++ idiom:

```cpp
#ifdef __cplusplus 
extern "C" {
#endif
    /* ... */
#ifdef __cplusplus
}
#endif
```

is to ensure that the symbols within are "unmangled" – that the compiler emits(创造，生产出) a binary file with their names undecorated, as a C compiler would do. As C language definitions are unmangled, the C++ compiler needs to avoid mangling references to these identifiers.

For example, the standard strings library, `<string.h>` usually contains something resembling:

```cpp
#ifdef __cplusplus
extern "C" {
#endif

void *memset (void *, int, size_t);
char *strcat (char *, const char *);
int   strcmp (const char *, const char *);
char *strcpy (char *, const char *);

#ifdef __cplusplus
}
#endif
```

Thus, code such as:

```c
if (strcmp(argv[1], "-x") == 0) 
    strcpy(a, argv[2]);
else 
    memset (a, 0, sizeof(a));
```

uses the correct, unmangled `strcmp` and `memset`. If the `extern` had not been used, the (SunPro) C++ compiler would produce code equivalent to:

```c
if (__1cGstrcmp6Fpkc1_i_(argv[1], "-x") == 0) 
    __1cGstrcpy6Fpcpkc_0_(a, argv[2]);
else 
    __1cGmemset6FpviI_0_ (a, 0, sizeof(a));
```

Since those **symbols** do not exist in the C runtime library (*e.g.* `libc`), link errors would result.

#### Standardised name mangling in C++

Though it would seem that standardised name mangling in the C++ language would lead to greater interoperability between compiler implementations, such a standardization by itself would not suffice to guarantee C++ compiler interoperability and it might even create a false impression that interoperability is possible and safe when it isn't. **Name mangling** is only one of several [application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface) (ABI) details that need to be decided and observed by a C++ implementation. Other ABI aspects like [exception handling](https://en.wikipedia.org/wiki/Exception_handling), [virtual table](https://en.wikipedia.org/wiki/Virtual_table) layout, structure and stack frame [padding](https://en.wikipedia.org/wiki/Data_structure_alignment), *etc.* also cause differing C++ implementations to be incompatible. Further, requiring a particular form of mangling would cause issues for systems where implementation limits (e.g., length of symbols) dictate a particular mangling scheme. A standardised *requirement* for name mangling would also prevent an implementation where mangling was not required at all — for example, a linker which understood the C++ language.

The [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882) therefore does not attempt to standardise name mangling. On the contrary, the *Annotated C++ Reference Manual* (also known as *ARM*, [ISBN](https://en.wikipedia.org/wiki/International_Standard_Book_Number) [0-201-51459-1](https://en.wikipedia.org/wiki/Special:BookSources/0-201-51459-1), section 7.2.1c) actively encourages the use of different mangling schemes to prevent linking when other aspects of the ABI, such as [exception handling](https://en.wikipedia.org/wiki/Exception_handling) and [virtual table](https://en.wikipedia.org/wiki/Virtual_table) layout, are incompatible.

Nevertheless, as detailed in the section above, on some platforms[[4\]](https://en.wikipedia.org/wiki/Name_mangling#cite_note-4) the full C++ ABI has been standardized, including name mangling.

#### Real-world effects of C++ name mangling

Because C++ symbols are routinely exported from [DLL](https://en.wikipedia.org/wiki/Dynamic-link_library) and [shared object](https://en.wikipedia.org/wiki/Shared_object) files, the name mangling scheme is not merely a compiler-internal matter. Different compilers (or different versions of the same compiler, in many cases) produce such binaries under different name decoration schemes, meaning that symbols are frequently unresolved if the compilers used to create the library and the program using it employed different schemes. For example, if a system with multiple C++ compilers installed (e.g., GNU GCC and the OS vendor's compiler) wished to install the [Boost C++ Libraries](https://en.wikipedia.org/wiki/Boost_C%2B%2B_Libraries), it would have to be compiled multiple times (once for GCC and once for the vendor compiler).

It is good for safety purposes that compilers producing **incompatible** object codes (codes based on different ABIs, regarding e.g., classes and exceptions) use different name mangling schemes. This guarantees that these incompatibilities are detected at the **linking** phase, not when executing the software (which could lead to obscure(模糊的，隐晦的) bugs and serious stability issues).

For this reason name decoration is an important aspect of any C++-related [ABI](https://en.wikipedia.org/wiki/Application_binary_interface).

#### Demangle via c++filt

```shell
$ c++filt _ZNK3MapI10StringName3RefI8GDScriptE10ComparatorIS0_E16DefaultAllocatorE3hasERKS0_
Map<StringName, Ref<GDScript>, Comparator<StringName>, DefaultAllocator>::has(StringName const&) const
```

#### Demangle via builtin GCC ABI

```c++
#include <stdio.h>
#include <stdlib.h>
#include <cxxabi.h>

int main() {
	const char *mangled_name = "_ZNK3MapI10StringName3RefI8GDScriptE10ComparatorIS0_E16DefaultAllocatorE3hasERKS0_";
	char *demangled_name;
	int status = -1;
	demangled_name = abi::__cxa_demangle(mangled_name, NULL, NULL, &status);
	printf("Demangled: %s\n", demangled_name);
	free(demangled_name);
	return 0;
}
```

Output:

```
Demangled: Map<StringName, Ref<GDScript>, Comparator<StringName>, DefaultAllocator>::has(StringName const&) const
```



### Python

In [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), mangling is used for "private" class members which are designated as such by giving them a name with two leading underscores and no more than one trailing underscore. For example, `__thing` will be mangled, as will `___thing` and `__thing_`, but `__thing__` and `__thing___` will not. Python's runtime does not restrict access to such members, the mangling only prevents name collisions if a derived class defines a member with the same name.

On encountering name mangled attributes, Python transforms these names by prepending a single underscore and the name of the enclosing class, for example:

```python
>>> class Test(object):
...     def __mangled_name(self):
...         pass
...     def normal_name(self):
...         pass
>>> t = Test()
>>> [attr for attr in dir(t) if 'name' in attr]
['_Test__mangled_name', 'normal_name']
```
# gcc.gnu [3.8 Options to Request or Suppress Warnings](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)



## Request or Suppress Warnings

You can request many specific warnings with options beginning with ‘`-W`’, for example `-Wimplicit` to request warnings on implicit declarations. Each of these specific warning options also has a negative form beginning ‘`-Wno-`’ to turn off warnings; for example, `-Wno-implicit`. This manual lists only one of the two forms, whichever is not the default. For further language-specific options also refer to [C++ Dialect Options](https://gcc.gnu.org/onlinedocs/gcc-10.3.0/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options) and [Objective-C and Objective-C++ Dialect Options](https://gcc.gnu.org/onlinedocs/gcc-10.3.0/gcc/Objective-C-and-Objective-C_002b_002b-Dialect-Options.html#Objective-C-and-Objective-C_002b_002b-Dialect-Options). Additional warnings can be produced by enabling the static analyzer; See [Static Analyzer Options](https://gcc.gnu.org/onlinedocs/gcc-10.3.0/gcc/Static-Analyzer-Options.html#Static-Analyzer-Options).

> NOTE: 
>
> 1、这总结的是"Request or Suppress Warnings"的模式



## `-Wall`

> NOTE: 
>
> 1、它并没有开启所有的warning，它只开启了部分warning
>
> 



## `-Wextra`



## stackoverflow [Suppress -Wunknown-pragmas warning in GCC](https://stackoverflow.com/questions/12842306/suppress-wunknown-pragmas-warning-in-gcc)



[A](https://stackoverflow.com/a/23412532)

I've run into this annoyance, too. According to the GCC manpage `-Wall` turns on `-Wunknown-pragmas` for you, so just manually disable it using `-Wno-unknown-pragmas` after `-Wall`.

There is a GCC feature request to make this work using `#pragma GCC diagnostic`:

1、[C++ preprocessor ignores #pragma GCC diagnostic](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431)





## TODO

stackoverflow [GCC does not honor 'pragma GCC diagnostic' to silence warnings [duplicate]](https://stackoverflow.com/questions/31509434/gcc-does-not-honor-pragma-gcc-diagnostic-to-silence-warnings)


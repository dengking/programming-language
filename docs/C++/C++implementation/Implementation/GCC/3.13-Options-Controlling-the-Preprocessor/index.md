# gcc-10.3.0 [3.13 Options Controlling the Preprocessor](https://gcc.gnu.org/onlinedocs/gcc-10.3.0/gcc/Preprocessor-Options.html)





## stackoverflow [How can I find the header files of the C programming language in Linux?](https://stackoverflow.com/questions/13079650/how-can-i-find-the-header-files-of-the-c-programming-language-in-linux)



[A](https://stackoverflow.com/a/13080065)

`gcc -H ...` will print the full path of every include file as a side-effect of regular compilation. Use `-fsyntax-only` in addition to get it not to create any output (it will still tell you if your program has errors).
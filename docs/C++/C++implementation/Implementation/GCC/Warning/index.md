# Warning in GCC

## `-Wall`

开启所有的warning。



## Suppress/silence warning

### stackoverflow [Suppress -Wunknown-pragmas warning in GCC](https://stackoverflow.com/questions/12842306/suppress-wunknown-pragmas-warning-in-gcc)



[A](https://stackoverflow.com/a/23412532)

I've run into this annoyance, too. According to the GCC manpage `-Wall` turns on `-Wunknown-pragmas` for you, so just manually disable it using `-Wno-unknown-pragmas` after `-Wall`.

There is a GCC feature request to make this work using `#pragma GCC diagnostic`:

- [C++ preprocessor ignores #pragma GCC diagnostic](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431)



## TODO

stackoverflow [GCC does not honor 'pragma GCC diagnostic' to silence warnings [duplicate]](https://stackoverflow.com/questions/31509434/gcc-does-not-honor-pragma-gcc-diagnostic-to-silence-warnings)


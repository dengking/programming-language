# Plateform macro

## stackoverflow [How to identify platform/compiler from preprocessor macros?](https://stackoverflow.com/questions/4605842/how-to-identify-platform-compiler-from-preprocessor-macros)



### [A](https://stackoverflow.com/a/4605893)

For **Mac OS**:

```cpp
#ifdef __APPLE__
```

For **MingW** on Windows:

```cpp
#ifdef __MINGW32__
```

For **Linux**:

```cpp
#ifdef __linux__
```

For other Windows compilers, check [this thread](https://stackoverflow.com/questions/430424/how-can-i-detect-the-operating-system-in-c-c) and [this](https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time/152067#152067) for several other compilers and architectures.

## sourceforge [Pre-defined Compiler Macros](https://sourceforge.net/p/predef/)

> NOTE: 非常全面


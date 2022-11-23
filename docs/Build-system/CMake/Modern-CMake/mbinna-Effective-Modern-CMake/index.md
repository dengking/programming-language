# [mbinna](https://gist.github.com/mbinna) / [Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)

## Getting Started

For a brief user-level introduction to CMake, watch C++ Weekly, Episode 78, [Intro to CMake](https://www.youtube.com/watch?v=HPMvU64RUTY) by Jason Turner. LLVM’s [CMake Primer](https://llvm.org/docs/CMakePrimer.html) provides a good high-level introduction to the CMake syntax. Go read it now.

After that, watch Mathieu Ropert’s CppCon 2017 talk [Using Modern CMake Patterns to Enforce a Good Modular Design](https://www.youtube.com/watch?v=eC9-iRN2b04) ([slides](https://github.com/CppCon/CppCon2017/blob/master/Tutorials/Using Modern CMake Patterns to Enforce a Good Modular Design/Using Modern CMake Patterns to Enforce a Good Modular Design - Mathieu Ropert - CppCon 2017.pdf)). It provides a thorough explanation of what modern CMake is and why it is so much better than “old school” CMake. The modular design ideas in this talk are based on the book [Large-Scale C++ Software Design](https://www.amazon.de/Large-Scale-Software-Addison-Wesley-Professional-Computing/dp/0201633620) by John Lakos. The next video that goes more into the details of modern CMake is Daniel Pfeifer’s C++Now 2017 talk [Effective CMake](https://www.youtube.com/watch?v=bsXLMQ6WgIk) ([slides](https://github.com/boostcon/cppnow_presentations_2017/blob/master/05-19-2017_friday/effective_cmake__daniel_pfeifer__cppnow_05-19-2017.pdf)).

This text is heavily influenced by Mathieu Ropert’s and Daniel Pfeifer’s talks.

If you are interested in the history and internal architecture of CMake, have a look at the article [CMake](http://www.aosabook.org/en/cmake.html) in the book [The Architecture of Open Source Applications](http://aosabook.org/en/index.html).
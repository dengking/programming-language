# [Scripting Commands](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html#id3)



## option[¶](https://cmake.org/cmake/help/latest/command/option.html#option)

> NOTE: 
>
> 补充内容: 
>
> stackoverflow [Adding command line options to CMake](https://stackoverflow.com/questions/5998186/adding-command-line-options-to-cmake)
>
> ```cmake
> //CMakeLists.txt
> option(MyOption "MyOption" OFF)
> 
> //Command line
> cmake -DMyOption=ON MyProjectFolder
> ```
>
> Note that `-DMyOption` must come before the path.
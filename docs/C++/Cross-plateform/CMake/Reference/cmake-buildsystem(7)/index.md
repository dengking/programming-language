# [cmake-buildsystem(7)](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id14)

## [Introduction](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id15)

> NOTE: 
>
> 一、提出了非常重要的概念: "target"，两类target:
>
> 1、[Binary Targets](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id16) : executable、library
>
> 2、"custom target containing custom commands"

A CMake-based buildsystem is organized as a set of high-level logical targets. Each target corresponds to an executable or library, or is a custom target containing custom commands. Dependencies between the targets are expressed in the buildsystem to determine the build order and the rules for regeneration in response to change.

> NOTE: 
>
> 一、dependency graph-topological sorting
>
> 从后门的内容可知: "Dependencies between binary targets are expressed using the [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries) command"

## [Binary Targets](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id16)



```cmake
add_library(archive archive.cpp zip.cpp lzma.cpp)
add_executable(zipapp zipapp.cpp)
target_link_libraries(zipapp archive)
```

## [Build Specification and Usage Requirements](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id22)



|                                                              |                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories) | [`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) | [`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options) |
| [`INCLUDE_DIRECTORIES`](https://cmake.org/cmake/help/latest/prop_tgt/INCLUDE_DIRECTORIES.html#prop_tgt:INCLUDE_DIRECTORIES) | [`COMPILE_DEFINITIONS`](https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html#prop_tgt:COMPILE_DEFINITIONS) | [`COMPILE_OPTIONS`](https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_OPTIONS.html#prop_tgt:COMPILE_OPTIONS) |
| [`INTERFACE_INCLUDE_DIRECTORIES`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_INCLUDE_DIRECTORIES.html#prop_tgt:INTERFACE_INCLUDE_DIRECTORIES) | [`INTERFACE_COMPILE_DEFINITIONS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_COMPILE_DEFINITIONS.html#prop_tgt:INTERFACE_COMPILE_DEFINITIONS) | [`INTERFACE_COMPILE_OPTIONS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_COMPILE_OPTIONS.html#prop_tgt:INTERFACE_COMPILE_OPTIONS) |

> NOTE: 
>
> 一、上面总结了command、target property之间的对应关系，关于这些target property的含义，在下面的 [Target Properties](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id23)[¶](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-properties) 章节中会进行非常详细的介绍

### [Target Properties](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id23)[¶](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-properties)



The contents of the [`INTERFACE_INCLUDE_DIRECTORIES`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_INCLUDE_DIRECTORIES.html#prop_tgt:INTERFACE_INCLUDE_DIRECTORIES), [`INTERFACE_COMPILE_DEFINITIONS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_COMPILE_DEFINITIONS.html#prop_tgt:INTERFACE_COMPILE_DEFINITIONS) and [`INTERFACE_COMPILE_OPTIONS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_COMPILE_OPTIONS.html#prop_tgt:INTERFACE_COMPILE_OPTIONS) target properties are *Usage Requirements* -- they specify content which consumers must use to correctly compile and link with the target they appear on. For any binary target, the contents of each `INTERFACE_` property on each target specified in a [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries) command is consumed:
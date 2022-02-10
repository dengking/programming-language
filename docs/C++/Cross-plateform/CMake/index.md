# CMake



## CMake概述

一、CMake描述如何构建一个project:

1、project

command: [project](https://cmake.org/cmake/help/latest/command/project.html)

一个project是可以包含多个target的

2、target，需要描述如何了来build target	

通过command [add_executable](https://cmake.org/cmake/help/latest/command/add_executable.html#id1) 、[add_library](https://cmake.org/cmake/help/latest/command/add_library.html#id1) 来向project中添加target



二、通过command来控制target property

比如:

1、options: 

[add_compile_options](https://cmake.org/cmake/help/latest/command/add_compile_options.html#add-compile-options) command adds options to the [`COMPILE_OPTIONS`](https://cmake.org/cmake/help/latest/prop_dir/COMPILE_OPTIONS.html#prop_dir:COMPILE_OPTIONS) directory property.

[add_link_options](https://cmake.org/cmake/help/latest/command/add_link_options.html)

2、[add_compile_definitions](https://cmake.org/cmake/help/latest/command/add_compile_definitions.html#add-compile-definitions) 

3、[add_definitions](https://cmake.org/cmake/help/latest/command/add_definitions.html)



三、定义target、向project中添加target

1、[add_custom_target](https://cmake.org/cmake/help/latest/command/add_custom_target.html#add-custom-target)

2、[add_executable](https://cmake.org/cmake/help/latest/command/add_executable.html#id1)

3、[add_library](https://cmake.org/cmake/help/latest/command/add_library.html#id1)



四、cmake中 [Project Commands](https://cmake.org/cmake/help/v3.21/manual/cmake-commands.7.html#id4)[¶](https://cmake.org/cmake/help/v3.21/manual/cmake-commands.7.html#project-commands) 用于对project、target进行控制



| 阶段       |                              |                                                           |                              | 参考                                                         |
| ---------- | ---------------------------- | --------------------------------------------------------- | ---------------------------- | ------------------------------------------------------------ |
| preprocess | `target_include_directories` | `INCLUDE_DIRECTORIES` <br>`INTERFACE_INCLUDE_DIRECTORIES` | `<PRIVATE|PUBLIC|INTERFACE>` | leimao [CMake: Public VS Private VS Interface](https://leimao.github.io/blog/CMake-Public-Private-Interface/)  <br> |
| compile    |                              |                                                           |                              |                                                              |
| link       |                              |                                                           |                              |                                                              |
| install    |                              |                                                           |                              |                                                              |

## [cmake](https://cmake.org/)



## leimao [CMake: Public VS Private VS Interface](https://leimao.github.io/blog/CMake-Public-Private-Interface/)

> NOTE: 
>
> 类比C++ public、private、protected specifier



## variable

### `PROJECT_BINARY_DIR`

参见: 

1、Step 1: A Basic Starting Point[¶](https://cmake.org/cmake/help/latest/guide/tutorial/A Basic Starting Point.html#step-1-a-basic-starting-point)

binary tree

## command

### `configure_file`

让CMake去configure一个文件，参见:

1、Step 1: A Basic Starting Point[¶](https://cmake.org/cmake/help/latest/guide/tutorial/A Basic Starting Point.html#step-1-a-basic-starting-point)
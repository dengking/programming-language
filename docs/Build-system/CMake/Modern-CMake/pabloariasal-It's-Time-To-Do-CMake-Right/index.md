# [Pablo Arias](https://pabloariasal.github.io/) [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)

My excitement was soon shattered(破碎) after discovering the lack of standard practices in CMake usage and specially the insufficient adoption of modern design patterns. This post explores the concepts of what is known as modern CMake, which advocates for abandoning a traditional variable-based approach for a more structured model based on so-called **targets**. My intention is to show how “new” (>= 3.0.0) features can be employed to reshape your CMake system into a more maintainable and intuitive alternative that actually makes sense.



Many of the concepts presented here find their roots in Daniel Pfeifer’s masterpiece [Effective CMake](https://youtu.be/rLopVhns4Zs). Daniel has practically become the Messiah of the modern CMake church, preaching best practices and guidelines in a time when the only standard is to not have one. Daniel, I am your prophet.



Enough preambles. Does this look familiar to you?

```cmake
find_package(Boost 1.55 COMPONENTS asio) 
list(APPEND INCLUDE_DIRS ${BOOST_INCLUDE_DIRS})
list(APPEND LIBRARIES ${BOOST_LIBRARIES})

include_directories(${INCLUDE_DIRS})
link_libraries(${LIBRARIES})
```

Don’t. Just don’t. This is wrong in so many dimensions. You are just blindly throwing stuff into a pot of include directories and compiler flags. There is no structure. There is no transparency. Not to mention that functions like `include_directories` work at the directory level and apply to all entities defined in scope.

And this isn’t even the real problem, what do you do with transitive dependencies? What about the order of linking? Yes, you need to take care about that yourself. The moment you need to deal with the dependencies of your dependencies is the moment your life needs to be reevaluated.

## Targets and Properties

CMake developers saw the aforementioned problems and introduced language features that allow you to better structure your projects. Modern CMake is all about targets and properties. Conceptually this isn’t complicated. Targets model the **components** of you application. An executable is a target, a library is a target. Your application is built as a collection of targets that depend on and use each other.

Targets have properties. Properties of a target are the source files it’s built from, the compiler options it requires, the libraries it links against. In modern CMake you create a list of targets and define the necessary properties on them.



## Build Requirements vs Usage Requirements

Target properties are defined in one of two scopes: **INTERFACE** and **PRIVATE**. Private properties are used *internally* to build the target, while interface properties are used *externally* by users of the target. In other words, interface properties model usage requirements, whereas private properties model build requirements of targets.

Target properties are defined in one of two scopes: **INTERFACE** and **PRIVATE**. Private properties are used *internally* to build the target, while interface properties are used *externally* by users of the target. In other words, interface properties model usage requirements, whereas private properties model build requirements of targets.

Interface properties have the prefix, wait for it, *INTERFACE_* prepended to them.

For example, the property **COMPILE_OPTIONS** encodes a list of options to be passed to the compiler when building the target. If a target must be built with all warnings enabled, for instance, this list should contain the option `-Wall`. This is a private property used only when building the target and won’t affect its users in any way.

On the other hand, the property **INTERFACE_COMPILE_FEATURES** stores which features must be supported by the compiler when building *users* of the target. For instance, if the public header of a library contains a variadic function template, this property should contain the feature `cxx_variadic_templates`. This instructs CMake that applications including this header will have to be built by a compiler that understands variadic templates.

Properties can also be specified as **PUBLIC**. Public properties are defined in both **PRIVATE** and **INTERFACE** scopes.

All of this is better understood with an example.



## libjsonutils

Imagine that you are writing a json utily library, *libjsonutils*, that parses json files from a provided location. Json files can be located on your local file system, as well as accessible via some URL.

The library has the following structure:

```shell
libjsonutils
├── CMakeLists.txt
├── include
│   └── jsonutils
│       └── json_utils.h
├── src
│   ├── file_utils.h
│   └── json_utils.cpp
└── test
    ├── CMakeLists.txt
    └── src
        └── test_main.cpp
```

We have a single public header, were we define the `loadJson()` function:

```c++
boost::optional<rapidjson::Document> loadJson(const std::string& url);
```
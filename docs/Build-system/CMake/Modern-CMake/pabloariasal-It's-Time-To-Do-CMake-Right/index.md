# pabloariasal [Pablo Arias](https://pabloariasal.github.io/) [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)

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



Now let’s define some properties on our target. Why not start with the include directories?

```cmake
target_include_directories(JSONUtils
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
```

Our headers are located in two different places: inside `src/`, which contains a utility header called `file_utils.h`, and in `include/`, where our public header `json_utils.h` lives. For building our library we need all headers in both locations (`json_utils.cpp` includes both), so **INCLUDE_DIRS** must contain `src/`, as well as `include/`.

On the other hand, users of *jsonutils* only need to know about the location of the public header `json_utils.h`, so **INTERFACE_INCLUDE_DIRS** only needs to contain `include/`, but not `src/`.

There is still a problem, though. While building jsonutils, `include/` is at `/home/pablo/libjsonutils/include/`, but after installing our library, it will be under `${CMAKE_INSTALL_PREFIX}/include/`. Therefore, the location of this directory needs to be different depending on whether we are building or installing the library. To solve this problem, we use [generator expressions](https://cmake.org/cmake/help/v3.5/manual/cmake-generator-expressions.7.html), which set the correct path depending on the situation.



## Leave `CMAKE_CXX_FLAGS` Alone

We can now continue by defining extra properties on our target. For example, it could be beneficial to treat warnings as errors:

```cmake
target_compile_options(JSONUtils PRIVATE -Werror)
```

Given that we use `constexpr` and `auto`, we could set the language standard to c++11:

```cmake
target_compile_features(JSONUtils PRIVATE cxx_std_11)
```

Note that there is no reason to manually append `-std=c++11` to **CMAKE_CXX_FLAGS**, let CMake do that for you! Stay away from variable land, model your requirements via properties.

## Model dependencies with `target_link_libraries`

Let’s think about our dependencies. First off all, we need boost, as we use optional. Additionally, in order to figure out if the passed string is an URL, we have to evaluate it against some regex, so we need boost::regex (yes I know c++11 introduces regex utilities but bear with me). Second, we need rapidjson.

In CMake, `target_link_libraries` is used to model dependencies between targets.

```
find_package(Boost 1.55 REQUIRED COMPONENTS regex)
find_package(RapidJSON 1.0 REQUIRED MODULE)

target_link_libraries(JSONUtils
    PUBLIC
        Boost::boost RapidJSON::RapidJSON
    PRIVATE
        Boost::regex
)
```
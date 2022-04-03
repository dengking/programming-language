# cmake boost



## stackoverflow [How to link C++ program with Boost using CMake](https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake)



### [A](https://stackoverflow.com/a/3917033/10173843)

In CMake you could use `find_package` to find libraries you need. There usually is a `FindBoost.cmake` along with your CMake installation.

As far as I remember, it will be installed to `/usr/share/cmake/Modules/` along with other find-scripts for common libraries. You could just check the documentation in that file for more information about how it works.

An example out of my head:

```scss
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ADD_EXECUTABLE( anyExecutable myMain.cpp )

TARGET_LINK_LIBRARIES( anyExecutable LINK_PUBLIC ${Boost_LIBRARIES} )
```

I hope this code helps.

- Here's the official [documentation about FindBoost.cmake](http://www.cmake.org/cmake/help/git-master/module/FindBoost.html).
- And the actual [FindBoost.cmake](https://github.com/Kitware/CMake/blob/master/Modules/FindBoost.cmake) (hosted on GitHub)





## github [Orphis](https://github.com/Orphis)/**[boost-cmake](https://github.com/Orphis/boost-cmake)**





## cmake docs [FindBoost](https://cmake.org/cmake/help/latest/module/FindBoost.html)

```cmake
find_package(Boost
  [version] [EXACT]      # Minimum or EXACT version e.g. 1.67.0
  [REQUIRED]             # Fail with error if Boost is not found
  [COMPONENTS <libs>...] # Boost libraries by their canonical name
                         # e.g. "date_time" for "libboost_date_time"
  [OPTIONAL_COMPONENTS <libs>...]
                         # Optional Boost libraries by their canonical name)
  )                      # e.g. "date_time" for "libboost_date_time"
```



## cliutils.gitlab [modern-cmake # Boost library](https://cliutils.gitlab.io/modern-cmake/chapters/packages/Boost.html)
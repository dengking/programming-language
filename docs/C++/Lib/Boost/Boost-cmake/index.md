# cmake boost



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

### Result Variables



### Cache variables



### Hints

This module reads hints about search locations from variables:

1、`BOOST_ROOT`, `BOOSTROOT`

Preferred installation prefix.



2、`BOOST_INCLUDEDIR`

Preferred include directory e.g. `<prefix>/include`.



3、`BOOST_LIBRARYDIR`

Preferred library directory e.g. `<prefix>/lib`.



4、`Boost_NO_SYSTEM_PATHS`

Set to `ON` to disable searching in locations not specified by these hint variables. Default is `OFF`.



5、`Boost_ADDITIONAL_VERSIONS`

List of Boost versions not known to this module. (Boost install locations may contain the version).



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



### [A](https://stackoverflow.com/a/6501329)

The following is my configuration:

```scss
cmake_minimum_required(VERSION 2.8)
set(Boost_INCLUDE_DIR /usr/local/src/boost_1_46_1)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_46_1/stage/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(main main.cpp)
target_link_libraries( main ${Boost_LIBRARIES} )
```



### [A](https://stackoverflow.com/a/43885372)

Adapting @MOnsDaR answer for modern CMake syntax with imported targets, this would be:

```scss
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(anyExecutable myMain.cpp)

target_link_libraries(anyExecutable Boost::program_options)
```

Note that it is not necessary to specify the include directories manually, since it is already taken care of through the imported target `Boost::program_options`.



## Boost hints / environment variable



一、`BOOST_ROOT` 

`BOOST_ROOT` 的命名和之前碰到的 `repository_root` 非常类似

二、`BOOST_LIBRARYDIR`

三、`BOOST_INCLUDEDIR`

### github [How to set BOOST_LIBRARYDIR or BOOST_ROOT? #123](https://github.com/exercism/cpp/issues/123)

I'd much prefer to use cmake in Cygwin, but that's not working either so at this point anything that works would be great since I just want to learn `C++` but can't currently run the tests, unfortunately.

The error message states:

*"No Boost libraries were found. You may need to set `BOOST_LIBRARYDIR` to the directory containing Boost libraries or `BOOST_ROOT` to the location of Boost."*

How do I set `BOOST_LIBRARYDIR` or `BOOST_ROOT`?



#### **[ScottHutchinson](https://github.com/ScottHutchinson)** # [A](https://github.com/exercism/cpp/issues/123#issuecomment-360922617)

```shell
cmake -G "Visual Studio 15" \
-DBOOST_ROOT:PATH=C:/local/boost_1_59_0 \
-DBOOST_INCLUDEDIR:PATH=C:/local/boost_1_59_0/boost \
-DBOOST_LIBRARYDIR:PATH=C:/local/boost_1_59_0/lib \
..
```



#### **[FirefoxMetzger](https://github.com/FirefoxMetzger)** # [A](https://github.com/exercism/cpp/issues/123#issuecomment-439693835)

> How do I set `BOOST_LIBRARYDIR` or `BOOST_ROOT`?

On Win10 this can be done as environment variables.

Here you can create two variables: 

1、One called `BOOST_ROOT` pointing to the directory that contains the "boost" folder, e.g., `C:\Program Files\boost\boost_1_68_0` 

2、Another called `BOOST_LIBRARYDIR` pointing to the location of the compiled boost libraries, e.g., `C:\Program Files\boost\boost_1_68_0\stage_x64`.

Note that `BOOST_LIBRARYDIR` may vary depending on how you have build / obtained boost. If you build it from source the easiest way is to specify `--stagedir=stage_x64`, or w/e naming convention makes sense for you, when calling `.\b2`.

You want to restart any console afterwards to make them pick up the changes to environment variables. You can verify via `echo $Env:BOOST_ROOT` and `echo $Env:BOOST_LIBRARYDIR`.

If CMake is up to date, it should pick up these environment variables as hints when searching for boost. You may also have to set

```
cmake_policy(SET CMP0074 NEW)
```

somewhere at the top of the project's `CMakeLists.txt`, to explicitly allow the use of environment variables as hints.

Hope this helps.

### stackoverflow [what is BOOST_ROOT?](https://stackoverflow.com/questions/9697926/what-is-boost-root)

I'm trying to build x64 version of http://code.google.com/p/quickfast/ That's how build scripts use %`BOOST_ROOT`%:

```perl
set SETUP_CHECKING=BOOST_ROOT=%BOOST_ROOT%
if not exist "%BOOST_ROOT%\boost" goto setup_is_bad

includes += $(BOOST_ROOT)/include

set RELEASE_PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Release;%MPC_ROOT%;%BOOST_ROOT%\lib;
```

> NOTE:
>
> 一、上述写法非常符合macOS上使用brew安装的boost



So build script expects following file/folders:

```shell
%BOOST_ROOT%\boost
$(BOOST_ROOT)/include
%BOOST_ROOT%\lib
```

I've downloaded/builded boost and there are no `lib` or `include` folder. That's what my `boost` folder contains:

```ruby
C:\bin\boost_1_49_0

14.03.2012  11:59    <DIR>          .
14.03.2012  11:59    <DIR>          ..
13.03.2012  16:17           249 856 b2.exe
13.03.2012  16:18    <DIR>          bin.v2
13.03.2012  16:17           249 856 bjam.exe
22.02.2012  18:25    <DIR>          boost
30.01.2012  21:30               866 boost-build.jam
30.01.2012  21:30             1 054 boost.css
30.01.2012  21:30             6 308 boost.png
30.01.2012  21:30            18 606 boostcpp.jam
30.01.2012  21:30             2 545 bootstrap.bat
13.03.2012  16:17             2 496 bootstrap.log
30.01.2012  21:30            10 324 bootstrap.sh
22.02.2012  18:05    <DIR>          doc
30.01.2012  21:30               794 index.htm
29.01.2012  16:58             5 556 index.html
30.01.2012  21:30               298 INSTALL
29.01.2012  16:58            10 423 Jamroot
22.02.2012  18:06    <DIR>          libs
30.01.2012  21:30             1 361 LICENSE_1_0.txt
13.03.2012  16:37             4 781 log.txt
22.02.2012  18:28    <DIR>          more
13.03.2012  16:17                76 project-config.jam
30.01.2012  21:30             2 757 rst.css
13.03.2012  16:18    <DIR>          stage
22.02.2012  18:24    <DIR>          status
22.02.2012  18:28    <DIR>          tools
```

Where can I find x64 boost version that contains `lib` `include` and `boost` folders?



#### [A](https://stackoverflow.com/a/9697993)

Include files are in folder `%BOOST_ROOT%\boost`
Libraries are in `%BOOST_ROOT%\libs`

Change your build script accordingly.

Example:

```makefile
%BOOST_ROOT%: c:\bin\boost_1_49_0
Includes: %BOOST_ROOT%\boost
Libs: %BOOST_ROOT%\stage\lib
```



### stackoverflow [CMake is not able to find BOOST libraries](https://stackoverflow.com/questions/24173330/cmake-is-not-able-to-find-boost-libraries)



#### [A](https://stackoverflow.com/a/33380087)

I'm using this to set up boost from cmake in my `CMakeLists.txt`. Try something similar (make sure to update paths to your installation of boost).

```scss
SET (BOOST_ROOT "/opt/boost/boost_1_57_0")
SET (BOOST_INCLUDEDIR "/opt/boost/boost-1.57.0/include")
SET (BOOST_LIBRARYDIR "/opt/boost/boost-1.57.0/lib")

SET (BOOST_MIN_VERSION "1.55.0")
set (Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost (version >= 1.55) required.")
else()
  message(STATUS "Setting up BOOST")
  message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
  message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)
```

This will either search default paths (`/usr`, `/usr/local`) or the path provided through the cmake variables (`BOOST_ROOT`, `BOOST_INCLUDEDIR`, `BOOST_LIBRARYDIR`). It works for me on cmake > 2.6.



### stackoverflow [Correct locations of BOOST_ROOT and BOOST_LIBRARY_DIR on OSX (brew)?](https://stackoverflow.com/questions/53490603/correct-locations-of-boost-root-and-boost-library-dir-on-osx-brew)



#### comments

I believe `1.68.0` is BOOST_ROOT. And that is the only variable you need to set. Also the variable should be set before find_package() or via the cmake-gui or ccmake – [drescherjm](https://stackoverflow.com/users/487892/drescherjm)

 



## github [Orphis](https://github.com/Orphis)/**[boost-cmake](https://github.com/Orphis/boost-cmake)**

> NOTE:
>
> 一、这个项目比较受欢迎



## cliutils.gitlab [modern-cmake # Boost library](https://cliutils.gitlab.io/modern-cmake/chapters/packages/Boost.html)


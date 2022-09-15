cmake_minimum_required(VERSION 3.24)
project(hello)

add_library(hello_wrapper SHARED hello.cpp)

FIND_PACKAGE(PythonInterp)

if(PYTHONINTERP_FOUND)
    FIND_PACKAGE(PythonInterp 3)
    FIND_PACKAGE(PythonLibs 3 REQUIRED)
else()
    message("Python not found")
endif()

SET(CMAKE_STATIC_LIBRARY_PREFIX_CXX "lib")
set(BOOST_INCLUDEDIR "D:/boost/boost_1_80_0/")
set(BOOST_LIBRARYDIR "D:/boost/boost_1_80_0/stage/lib/")

INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR} ${PYTHON_INCLUDE_DIRS})
# LINK_LIBRARIES(${BOOST_LIBRARYDIR} ${PYTHON_LIBRARIES}) # Deprecated but so convenient!

target_link_directories(hello_wrapper PRIVATE ${BOOST_LIBRARYDIR})

find_library(Boost_LIBRARIES NAMES boost_python38-vc143-mt-gd-x64-1_80 HINTS ${BOOST_LIBRARYDIR})

target_link_libraries(hello_wrapper ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

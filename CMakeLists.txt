cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

set(Headers 
    src/Criterion.h
    src/Criteria.h
    src/Performance.h
    src/AtomicMCDAObject.h
    src/Category.h
    )

set(Sources 
    src/Criterion.cpp
    src/Criteria.cpp
    src/Performance.cpp
    src/AtomicMCDAObject.cpp
    src/Category.cpp
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

add_executable(Test test/TestMain.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable


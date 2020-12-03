cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

set(Headers 
    src/example.h
    )

set(Sources 
    src/example.cpp
    )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

add_executable(Test test/test_example.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable
cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

add_executable(Test test/test_example.cpp src/example.cpp src/example.h) # add this executable

target_link_libraries(Test PRIVATE gtest) # link google test to this executable
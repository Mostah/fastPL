cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

set(Headers 
    src/CriterionValue.h
    src/AtomicMCDAObject.h
    )

set(Sources 
    src/CriterionValue.cpp
    src/AtomicMCDAObject.cpp
    )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

add_executable(Test test/TestMain.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable
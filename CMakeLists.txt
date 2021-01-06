cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

set(Headers 
    src/Criterion.h
    src/Criteria.h
    src/Perf.h
    src/Performance.h
    src/PerformanceTable.h
    src/AtomicMCDAObject.h
    src/Category.h
    src/Categories.h
    src/utils.h
    src/AlternativePerformance.h
    )

set(Sources 
    src/Criterion.cpp
    src/Criteria.cpp
    src/Perf.cpp
    src/Performance.cpp
    src/PerformanceTable.cpp
    src/AtomicMCDAObject.cpp
    src/Category.cpp
    src/Categories.cpp
    src/AlternativePerformance.cpp
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

add_executable(Test test/TestMain.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable


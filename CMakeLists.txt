cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

add_subdirectory(doxygen) # add doxygen doc generator

set(Headers 
    include/types/Criterion.h
    include/types/Criteria.h
    include/types/Perf.h
    include/types/Performance.h
    include/types/PerformanceTable.h
    include/types/AtomicMCDAObject.h
    include/types/Category.h
    include/types/Categories.h
    include/utils.h
    )

set(Sources 
    src/types/Criterion.cpp
    src/types/Criteria.cpp
    src/types/Perf.cpp
    src/types/Performance.cpp
    src/types/PerformanceTable.cpp
    src/types/AtomicMCDAObject.cpp
    src/types/Category.cpp
    src/types/Categories.cpp
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

add_executable(Test test/TestMain.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable


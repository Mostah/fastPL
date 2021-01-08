cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

# add external directories
add_subdirectory(extsrc/googletest)
add_subdirectory(extsrc/spdlog)
add_subdirectory(extsrc/pugixml)

set(Headers 
    src/utils.h
    src/Criterion.h
    src/Criteria.h
    src/Perf.h
    src/Performance.h
    src/PerformanceTable.h
    src/AtomicMCDAObject.h
    src/Category.h
    src/Categories.h
    src/DataGenerator.h
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
    src/DataGenerator.cpp
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files

# Add executables
add_executable(Test test/TestMain.cpp)
add_executable(Main src/main.cpp)

# link librairies to executables
target_link_libraries(Test Core gtest) 
target_link_libraries(Test Core spdlog::spdlog_header_only) 
target_link_libraries(Main Core spdlog::spdlog_header_only)
target_link_libraries(Test Core pugixml)
target_link_libraries(Main Core pugixml)

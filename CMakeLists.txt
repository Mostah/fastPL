cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

# add external directories
add_subdirectory(extsrc/googletest)
add_subdirectory(extsrc/spdlog)
add_subdirectory(extsrc/pugixml)
add_subdirectory(extsrc/yaml-cpp)

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
    include/types/DataGenerator.h
    include/types/AlternativesPerformance.h
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
    src/types/DataGenerator.cpp
    src/types/AlternativesPerformance.cpp
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

target_link_libraries(Test Core yaml-cpp)
target_link_libraries(Main Core yaml-cpp)

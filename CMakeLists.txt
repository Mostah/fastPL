cmake_minimum_required(VERSION 3.14) # version can be different

project(fastPL) #name of your project

# add external directories
add_subdirectory(extsrc/googletest)
add_subdirectory(extsrc/spdlog)
add_subdirectory(extsrc/pugixml)
add_subdirectory(extsrc/yaml-cpp)
add_subdirectory(extsrc/ortools)

add_subdirectory(doc) # add doxygen doc generator

set(Headers 
    include/learning/ProfileInitializer.h
    include/types/MRSortModel.h
    include/utils.h
    include/types/Criterion.h
    include/types/Criteria.h
    include/types/Perf.h
    include/types/PerformanceTable.h
    include/types/Profiles.h
    include/types/Category.h
    include/types/Categories.h
    include/types/DataGenerator.h
    include/types/AlternativesPerformance.h
    include/learning/LinearSolver.h
    include/learning/WeightUpdater.h
    include/app.h
    )

set(Sources 
    src/learning/ProfileInitializer.cpp
    src/types/MRSortModel.cpp
    src/types/Criterion.cpp
    src/types/Criteria.cpp
    src/types/Perf.cpp
    src/types/PerformanceTable.cpp
    src/types/Profiles.cpp
    src/types/Category.cpp
    src/types/Categories.cpp
    src/types/DataGenerator.cpp
    src/types/AlternativesPerformance.cpp
    src/learning/LinearSolver.cpp
    src/learning/WeightUpdater.cpp
    src/app.cpp
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )

add_library(Core ${Headers} ${Sources})   # build a library with our header and source files
target_link_libraries(Core spdlog::spdlog_header_only)
target_link_libraries(Core yaml-cpp)
target_link_libraries(Core pugixml)
target_link_libraries(Core ortools::ortools)


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

target_link_libraries(Main Core ortools::ortools)
target_link_libraries(Test Core ortools::ortools)
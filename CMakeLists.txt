cmake_minimum_required(VERSION 3.12) # version can be different

project(fastPL) #name of your project

add_subdirectory(extsrc/googletest) # add googletest subdirectory

# Not using anymore since forked pugixml git
# set(PUGIXML_HEADER_FILE ../extsrc/pugixml/src/pugiconfig.hpp)
# set(OTHER  "s@\/\/ #define PUGIXML_HEADER_ONLY@#define PUGIXML_HEADER_ONLY@")
# uncommenting relevant line of pugixml package, adding the executable
# execute_process(COMMAND sed -i "" ${OTHER} ${PUGIXML_HEADER_FILE} )

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

add_executable(Test test/TestMain.cpp) # add the executable

target_link_libraries(Test Core gtest) # link librairies to the executable


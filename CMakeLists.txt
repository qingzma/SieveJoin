cmake_minimum_required(VERSION 3.14)
project(QuasiJoin)

if(WIN32)
    message(WARNING "This code is not developed on Windows, and the behavior of this code on Windows is not checked.")
endif(WIN32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(qdb "")
target_include_directories(qdb PUBLIC "include")
target_sources(qdb PRIVATE
    util/bloom_filter.h
    table/tpch_table_nation.h
    table/tpch_table_nation.cc
    table/tpch_table_nation.h
    table/tpch_table_nation.cc)
set_target_properties(qdb PROPERTIES LINKER_LANGUAGE CXX)

add_executable(qentry "")
target_include_directories(qentry PUBLIC "include")
target_sources(qentry PRIVATE db/db_entry.cpp)
target_link_libraries(qentry qdb)

enable_testing()
add_executable(qtests "")
target_include_directories(qtests PUBLIC "include")
target_sources(qtests
    PRIVATE
    util/bloom_filter_test.cpp
    table/tpch_table_nation_test.cc
)
target_link_libraries(qtests qdb GTest::gtest_main)

if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
    add_compile_definitions(HAVE_TPCH_01_NATION="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
    message("file found")
endif()

include(GoogleTest)
gtest_discover_tests(qtests)

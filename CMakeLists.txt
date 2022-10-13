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
target_include_directories(qdb PUBLIC "include" ${CMAKE_SOURCE_DIR})
target_sources(qdb PRIVATE
    util/bloom_filter.h
    util/Timer.h
    table/tpch_table_nation.h
    table/tpch_table_nation.cc
    table/tpch_table_nation.h
    table/tpch_table_nation.cc
    table/tpch_table_customer.h
    table/tpch_table_customer.cc
    table/tpch_table_lineitem.h
    table/tpch_table_lineitem.cc
    table/tpch_table_part.h
    table/tpch_table_part.cc)
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
    table/tpch_table_customer_test.cc

    table/tpch_table_lineitem_test.cc
)
target_link_libraries(qtests qdb GTest::gtest_main)

if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
    add_compile_definitions(HAVE_TPCH_01_NATION="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/customer.tbl")
    add_compile_definitions(HAVE_TPCH_01_CUSTOMER="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/customer.tbl")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/lineitem.tbl")
    add_compile_definitions(HAVE_TPCH_01_LINEITEM="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/lineitem.tbl")
endif()

include(GoogleTest)
gtest_discover_tests(qtests)

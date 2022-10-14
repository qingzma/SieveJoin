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

# add_subdirectory(third_party/tclap-1.4)
include_directories(third_party/tclap-1.4/include)

# FetchContent_Declare(
# tclap
# GIT_REPOSITORY https://github.com/mirror/tclap.git
# GIT_TAG 1.4
# )
# FetchContent_MakeAvailable(tclap)
add_library(qdb "")
target_include_directories(qdb PUBLIC "include" ${CMAKE_SOURCE_DIR})
target_sources(qdb
    PUBLIC
    util/bloom_filter.h
    util/Timer.h
    util/Timer.cc
    util/options.cc
    table/tpch_table_nation.h
    table/tpch_table_nation.cc
    table/tpch_table_nation.h
    table/tpch_table_nation.cc
    table/tpch_table_customer.h
    table/tpch_table_customer.cc
    table/tpch_table_lineitem.h
    table/tpch_table_lineitem.cc
    table/tpch_table_orders.h
    table/tpch_table_orders.cc
    table/tpch_table_part.h
    table/tpch_table_part.cc
    table/tpch_table_partsupp.h
    table/tpch_table_partsupp.cc
    table/tpch_table_region.h
    table/tpch_table_region.cc
    table/tpch_table_supplier.h
    table/tpch_table_supplier.cc
)
set_target_properties(qdb PROPERTIES LINKER_LANGUAGE CXX)

add_executable(qentry "")
target_link_libraries(qentry qdb)

target_include_directories(qentry PUBLIC "include")
target_sources(qentry
    PUBLIC
    db/db_entry.cpp
    db/qx.h
    db/qx.cpp
)

option(BUILD_QJOIN_TESTS "build unit tests" TRUE)

if(BUILD_QJOIN_TESTS)
    enable_testing()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
        add_compile_definitions(HAVE_TPCH_01_NATION="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/nation.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/customer.tbl")
        add_compile_definitions(HAVE_TPCH_01_CUSTOMER="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/customer.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/lineitem.tbl")
        add_compile_definitions(HAVE_TPCH_01_LINEITEM="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/lineitem.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/orders.tbl")
        add_compile_definitions(HAVE_TPCH_01_ORDERS="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/orders.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/part.tbl")
        add_compile_definitions(HAVE_TPCH_01_PART="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/part.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/partsupp.tbl")
        add_compile_definitions(HAVE_TPCH_01_PARTSUPP="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/partsupp.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/region.tbl")
        add_compile_definitions(HAVE_TPCH_01_REGION="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/region.tbl")
    endif()

    if(EXISTS "${PROJECT_SOURCE_DIR}/data/tpch/x0.1/supplier.tbl")
        add_compile_definitions(HAVE_TPCH_01_SUPPLIER="${PROJECT_SOURCE_DIR}/data/tpch/x0.1/supplier.tbl")
    endif()

    add_executable(qtests "")
    target_include_directories(qtests PUBLIC "include")
    target_link_libraries(qtests PUBLIC qdb GTest::gtest_main)
    target_sources(qtests
        PRIVATE
        util/bloom_filter_test.cpp
        table/tpch_table_nation_test.cc
        table/tpch_table_customer_test.cc
        table/tpch_table_orders_test.cc
        table/tpch_table_part_test.cc
        table/tpch_table_partsupp_test.cc
        table/tpch_table_region_test.cc
        table/tpch_table_supplier_test.cc
        table/tpch_table_lineitem_test.cc
    )

    include(GoogleTest)
    gtest_discover_tests(qtests)
endif()

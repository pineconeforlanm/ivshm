cmake_minimum_required(VERSION 3.5...4.2.3)

if (${CMAKE_VERSION} VERSION_LESS 3.5)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

project(
        ivshm
        VERSION 0.1.0.0
        DESCRIPTION "IVSHM (Inter-VM Shared Memory) v2 - High-speed inter-virtual machine communication component based on shared memory"
        HOMEPAGE_URL "https://codeup.sinsegye.com.cn/lanmeng/ivshm-v2.git"
        LANGUAGES CXX C
)

message(STATUS "${PROJECT_NAME} VERSION: ${PROJECT_VERSION}")

include(cmake/compile_options.cmake)

option(BUILD_EXAMPLES "Build example programs" OFF)
option(BUILD_TESTS "Build test programs" OFF)

if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

add_subdirectory(bench)
add_subdirectory(docs)
add_subdirectory(src)

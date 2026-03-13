cmake_policy(SET CMP0077 NEW)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif ()

option(USE_CONAN_DEPS "Use Conan for dependency management" OFF)

if (USE_CONAN_DEPS)
    message(STATUS "Build Mode: [CONAN] - Resolving dependencies via Conan")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_EXTRA_CXX_FLAGS}")

if (ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_COMMON_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMMON_FLAGS} -coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

function(ensure_directory dir_path)
    if ("${dir_path}" STREQUAL "")
        message(WARNING "ensure_directory called with empty path!")
        return()
    endif ()

    if (NOT EXISTS "${dir_path}")
        file(MAKE_DIRECTORY "${dir_path}")
    endif ()
endfunction()

set(PROTO_GENERATE_OUT_PATH "${PROTO_GEN_PATH}" CACHE PATH "Path for proto files")
set(VERSION_GENERATE_OUT_PATH "${VERSION_GEN_PATH}" CACHE PATH "Path for version files")
set(SHARED_GENERATE_OUT_DIR "${GENERATED_OUT_DIR}" CACHE PATH "Root dir for generated files")

if (NOT PROTO_GENERATE_OUT_PATH)
    set(PROTO_GENERATE_OUT_PATH "${CMAKE_BINARY_DIR}/shared/proto")
endif ()

if (NOT VERSION_GENERATE_OUT_PATH)
    set(VERSION_GENERATE_OUT_PATH "${CMAKE_BINARY_DIR}/shared/version")
endif ()

ensure_directory("${PROTO_GENERATE_OUT_PATH}")
ensure_directory("${VERSION_GENERATE_OUT_PATH}")

include(FindPackageHandleStandardArgs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(${CMAKE_SOURCE_DIR}/cmake/project_generate_version.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/custom_executable.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/find_library.cmake)

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_TAG_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
endif()

if(NOT GIT_TAG_VERSION)
    set(GIT_TAG_VERSION "0.1.0-unknown")
endif()

string(REGEX REPLACE "^v" "" CLEAN_VERSION "${GIT_TAG_VERSION}")

set(${PROJECT_NAME}_PACKAGE_VERSION "${CLEAN_VERSION}" CACHE STRING "Project version for packaging")

message(STATUS "${PROJECT_NAME} Packaging version: ${${PROJECT_NAME}_PACKAGE_VERSION}")
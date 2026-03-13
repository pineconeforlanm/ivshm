function(generate_version_header)
    set(VERSION_TEMPLATE "${CMAKE_SOURCE_DIR}/cmake/version.h.in")

    if (NOT PROJECT_NAME)
        message(FATAL_ERROR "generate_version_header() must be called after project()")
    endif ()

    if (NOT EXISTS "${VERSION_TEMPLATE}")
        message(FATAL_ERROR "Version template NOT FOUND at: ${VERSION_TEMPLATE}")
    endif()

    # 1. 准备项目变量
    string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)

    # 2. 确定输出目录
    set(target_out_dir "${ARGV0}")
    if ("${target_out_dir}" STREQUAL "")
        set(target_out_dir "${VERSION_GENERATE_OUT_PATH}")
    endif()
    if ("${target_out_dir}" STREQUAL "")
        set(target_out_dir "${CMAKE_BINARY_DIR}/shared/version")
    endif()

    file(MAKE_DIRECTORY "${target_out_dir}")

    # 3. 获取 Git 信息
    find_package(Git QUIET)
    if (GIT_FOUND)
        # Hash
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
                OUTPUT_VARIABLE PROJECT_LATEST_COMMIT_HASH
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                ERROR_QUIET
        )
        # Branch
        execute_process(
                COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
                OUTPUT_VARIABLE PROJECT_BRANCH_NAME
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                ERROR_QUIET
        )
        # Describe
        execute_process(
                COMMAND ${GIT_EXECUTABLE} describe --tags --always
                OUTPUT_VARIABLE PROJECT_GIT_DESCRIBE
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                ERROR_QUIET
        )
        # Dirty Check
        execute_process(
                COMMAND ${GIT_EXECUTABLE} diff --quiet
                RESULT_VARIABLE GIT_DIRTY_CODE
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        if(GIT_DIRTY_CODE EQUAL 0)
            set(PROJECT_GIT_DIRTY_BOOL "false")
            set(PROJECT_GIT_DIRTY_STR "")
        else()
            set(PROJECT_GIT_DIRTY_BOOL "true")
            set(PROJECT_GIT_DIRTY_STR "-dirty")
        endif()
    endif ()

    # 兜底变量
    if (NOT PROJECT_LATEST_COMMIT_HASH)
        set(PROJECT_LATEST_COMMIT_HASH "unknown")
        set(PROJECT_GIT_DIRTY_BOOL "false")
    endif ()
    if (NOT PROJECT_BRANCH_NAME)
        set(PROJECT_BRANCH_NAME "unknown")
    endif ()
    if (NOT PROJECT_GIT_DESCRIBE)
        set(PROJECT_GIT_DESCRIBE "unknown")
    endif ()

    # 4. 构建环境信息
    string(TIMESTAMP PROJECT_COMPILE_TIME "%Y-%m-%d %H:%M:%S")
    if (WIN32)
        set(PROJECT_BUILD_USERNAME "$ENV{USERNAME}")
    else ()
        set(PROJECT_BUILD_USERNAME "$ENV{USER}")
    endif ()

    set(PROJECT_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
    if(NOT PROJECT_BUILD_TYPE)
        set(PROJECT_BUILD_TYPE "None")
    endif()

    set(PROJECT_COMPILER_INFO "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

    # 5. 生成文件
    set(VERSION_HEADER_PATH "${target_out_dir}/${PROJECT_NAME}_version.h")
    configure_file(
            "${VERSION_TEMPLATE}"
            "${VERSION_HEADER_PATH}"
            @ONLY
    )
endfunction()
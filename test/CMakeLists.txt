project(run_all_unittest VERSION ${CMAKE_PROJECT_VERSION} DESCRIPTION "learn c/c++ base example" LANGUAGES CXX C)

set(PREFIX_NAME "TEST_PINECONE")

file(GLOB_RECURSE ${PREFIX_NAME}_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

file(GLOB_RECURSE ${PREFIX_NAME}_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

if(ENABLE_COVERAGE)
    set(BUILD_TESTING ON CACHE BOOL "Build tests for coverage" FORCE)
endif()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_CONFIG_PATH="${CMAKE_SOURCE_DIR}")

target_sources(${PROJECT_NAME} PUBLIC ${${PREFIX_NAME}_SOURCES} ${${PREFIX_NAME}_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include" ${VERSION_GENERATE_OUT_PATH})

target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        stdc++exp
        ivshm
        GTest::gtest_main
        GTest::gtest
        Threads::Threads
)

add_test(NAME run_all_unittest COMMAND ${PROJECT_NAME})

install(TARGETS run_all_unittest RUNTIME DESTINATION bin)

generate_version_header()

if(ENABLE_COVERAGE)
    find_program(GCOVR_PATH gcovr)

    if(GCOVR_PATH)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                # 修复 1：使用双引号包裹 echo 的内容，避免括号被 Shell 解析
                COMMAND ${CMAKE_COMMAND} -E echo "Cleaning old coverage data (.gcda files)..."

                # 修复 2：给 "*.gcda" 加双引号，防止通配符被 Shell 提前展开
                COMMAND find . -name "*.gcda" -delete || true

                # 运行测试
                COMMAND ${CMAKE_COMMAND} -E echo "Running tests to collect coverage data..."
                COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R ${PROJECT_NAME}

                # 创建报告目录
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage_report

                # 生成 HTML 报告
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} .
                --gcov-executable gcov
                --filter "${CMAKE_SOURCE_DIR}/src/"
                --filter "${CMAKE_SOURCE_DIR}/include/"
                --html-details ${CMAKE_BINARY_DIR}/coverage_report/index.html
                --print-summary

                COMMENT "Building ${PROJECT_NAME} and automatically generating coverage report"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                VERBATIM # 强烈建议加上这个，让 CMake 自动处理转义
        )

        message(STATUS "Post-build coverage reporting enabled for ${PROJECT_NAME}")
    else()
        message(WARNING "gcovr not found, coverage report will not be auto-generated.")
    endif()
endif()
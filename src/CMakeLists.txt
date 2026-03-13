project(${PROJECT_NAME} VERSION ${CMAKE_PROJECT_VERSION} DESCRIPTION ${PROJECT_DESCRIPTION} LANGUAGES CXX C)

set(PREFIX_NAME "IVSHM")

file(GLOB_RECURSE ${PREFIX_NAME}_HEADERS
        "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h"
)
file(GLOB_RECURSE ${PREFIX_NAME}_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

if(USE_CONAN_DEPS)
    set(COMMON_LIBRARIES
            stdc++exp
            boost::boost
            magic_enum::magic_enum
            liburing::liburing
            yalantinglibs::yalantinglibs
    )
else ()
    set(COMMON_LIBRARIES
            stdc++exp
            Boost::boost
            magic_enum::magic_enum
            uring
            yalantinglibs::yalantinglibs
    )
endif()


function(create_library target_type sources headers)
    set(is_shared FALSE)
    if(${target_type} STREQUAL "SHARED")
        set(target_name ${PROJECT_NAME})
        set(is_shared TRUE)
    else()
        set(target_name ${PROJECT_NAME}_static)
    endif()

    add_library(${target_name} ${target_type} ${sources} ${headers})

    target_compile_definitions(${target_name}
            PUBLIC
            $<$<PLATFORM_ID:Windows>:IPC_OS_WINDOWS>
            $<$<PLATFORM_ID:Linux>:IPC_OS_LINUX>
            $<$<BOOL:${is_shared}>:IVSHM_IS_DLL>
            PRIVATE
            $<$<BOOL:${is_shared}>:IVSHM_EXPORTS>
    )

    set_target_properties(${target_name} PROPERTIES
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            OUTPUT_NAME "${PROJECT_NAME}"
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(${target_name} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    target_link_libraries(${target_name} PUBLIC ${COMMON_LIBRARIES})

    include(GNUInstallDirs)
    install(TARGETS ${target_name}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()

create_library("SHARED" "${${PREFIX_NAME}_SOURCES}" "${${PREFIX_NAME}_HEADERS}")
create_library("STATIC" "${${PREFIX_NAME}_SOURCES}" "${${PREFIX_NAME}_HEADERS}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_program(GCOVR_PATH gcovr)

    if(GCOVR_PATH)
        add_custom_target(coverage
                COMMAND ${CMAKE_COMMAND} -E echo "Running tests and generating coverage summary..."
                COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} . --filter src/ --filter include/
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating console coverage summary"
        )

        add_custom_target(coverage-html
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/coverage_report
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} .
                --filter src/ --filter include/
                --html-details ${CMAKE_CURRENT_BINARY_DIR}/coverage_report/index.html
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating detailed HTML coverage report"
        )

        add_custom_target(coverage-xml
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} .
                --filter src/ --filter include/
                --xml ${CMAKE_CURRENT_BINARY_DIR}/coverage.xml
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    else()
        message(WARNING "gcovr not found. Coverage targets will not be created.")
    endif()
endif()

generate_version_header()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "pineconeforlanm@outlook.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lanmeng <pineconeforlanm@outlook.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.29)")
set(CPACK_PACKAGE_DESCRIPTION "Example project with library (static+shared), binary, config, and docs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "pinecone library")
set(CPACK_PACKAGE_VENDOR "pineconeforlanm")

include(CPack)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_SOURCE_DIR}/README.md ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION share/doc/ivshm)

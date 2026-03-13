function(add_exp_target DIR_NAME)
    set(TARGET_NAME "exp_${PROJECT_NAME}_${DIR_NAME}")
    set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DIR_NAME}")

    file(GLOB_RECURSE SRC_FILES
            "${SOURCE_DIR}/*.cc" "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h"
    )

    if(NOT SRC_FILES)
        message(WARNING "No source files found in ${SOURCE_DIR}, skipping...")
        return()
    endif()

    add_executable(${TARGET_NAME} ${SRC_FILES})

    target_compile_definitions(${TARGET_NAME} PUBLIC DEFAULT_CONFIG_PATH="${CMAKE_SOURCE_DIR}")
    target_include_directories(${TARGET_NAME} PUBLIC
            "${CMAKE_SOURCE_DIR}/include"
            ${VERSION_GENERATE_OUT_PATH}
            "${SOURCE_DIR}"
    )

    target_link_libraries(${TARGET_NAME} PUBLIC stdc++exp ivshm)

    if(COMMAND generate_version_header)
        generate_version_header()
    endif()
endfunction()

macro(link_exp_target DIR_NAME)
    set(TARGET_NAME "exp_${PROJECT_NAME}_${DIR_NAME}")
    if(TARGET ${TARGET_NAME})
        target_link_libraries(${TARGET_NAME} PUBLIC ${ARGN})
    else()
        message(FATAL_ERROR "Target ${TARGET_NAME} not found! Call add_exp_target first.")
    endif()
endmacro()
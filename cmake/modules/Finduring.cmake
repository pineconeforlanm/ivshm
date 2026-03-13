if (LINUX)
    if (USE_CONAN_DEPS)
        find_package(liburing REQUIRED CONFIG)
        if (NOT TARGET uring::uring)
            add_library(uring::uring INTERFACE IMPORTED)
            target_link_libraries(uring::uring INTERFACE liburing::liburing)
        endif ()
    else ()
        find_package(PkgConfig REQUIRED)
        pkg_search_module(URING liburing)
        if (URING_FOUND)
            if (NOT TARGET uring::uring)
                add_library(uring::uring INTERFACE IMPORTED)

                set_target_properties(uring::uring PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${URING_INCLUDE_DIRS}"
                        INTERFACE_COMPILE_OPTIONS "${URING_CFLAGS_OTHER}"
                )
                target_link_libraries(uring::uring INTERFACE ${URING_LINK_LIBRARIES})
            endif ()
        else ()
            message(FATAL_ERROR "liburing not found via pkg-config. Please install liburing-dev.")
        endif ()
    endif ()

    if (TARGET uring::uring AND NOT TARGET liburing::liburing)
        add_library(liburing::liburing ALIAS uring::uring)
    endif ()
endif ()

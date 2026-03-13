pkg_search_module(GLIB REQUIRED glib-2.0)
if(GLIB_FOUND)
    if(NOT TARGET glib::glib)
        add_library(glib::glib INTERFACE IMPORTED)
        set_target_properties(glib::glib PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${GLIB_INCLUDE_DIRS}"
                INTERFACE_COMPILE_OPTIONS "${GLIB_CFLAGS_OTHER}"
        )

        target_link_libraries(glib::glib INTERFACE ${GLIB_LINK_LIBRARIES})
    endif()
endif()
set(DOXYGEN_HAVE_DOT YES)        # 开启 Graphviz 支持
set(DOXYGEN_CALL_GRAPH YES)      # 生成函数调用图
set(DOXYGEN_CALLER_GRAPH YES)    # 生成被调用图

find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")

    # --- 关键：配置 Awesome 布局 ---
    # 必须开启 TREEVIEW 才能发挥这个主题的最大效果
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_COLORSTYLE "LIGHT") # Doxygen 1.9.5+ 必须设置为 LIGHT

    # 引用下载的 CSS 文件 (这里使用 Sidebar-Only 布局，更现代)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
            "${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css"
            "${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome-sidebar-only.css"
    )

    # --- 进阶支持：Graphviz ---
    find_program(DOT_EXECUTABLE dot)
    if(DOT_EXECUTABLE)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_DOT_IMAGE_FORMAT "svg") # 矢量图更清晰
        set(DOXYGEN_DOT_TRANSPARENT YES)
    endif()

    # --- 创建生成目标 ---
    # 注意：输入目录直接作为参数传入，避免之前那个警告
    doxygen_add_docs(docs
            "${CMAKE_SOURCE_DIR}/include"
            "${CMAKE_SOURCE_DIR}/src"
            "${CMAKE_SOURCE_DIR}/README.md"
            COMMENT "Generating Modernized API Documentation"
    )
endif()

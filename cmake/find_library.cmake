set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_program(SPHINX_EXECUTABLE NAMES sphinx-build
        HINTS
        $ENV{SPHINX_DIR}
        HINTS ${SPHINX_ROOT}/bin
        PATH_SUFFIXES bin
        DOC "Sphinx documentation generator"
)

if (LINUX)
    find_package(PkgConfig REQUIRED)
    find_package(Doxygen REQUIRED)
    find_package(Perl REQUIRED)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    if(USE_CONAN_DEPS)
        find_package(liburing REQUIRED)
    else ()
        find_package(uring REQUIRED)
    endif ()
    find_package(Threads REQUIRED)
    message(STATUS "Found liburing: ${URING_VERSION}")
endif ()

add_subdirectory(thirdparty)

#find_package(yalantinglibs CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(magic_enum REQUIRED)

message(STATUS "Found Boost: ${Boost_VERSION}")
message(STATUS "Found magic_enum: ${magic_enum_VERSION}")

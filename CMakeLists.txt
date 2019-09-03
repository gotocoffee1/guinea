project(guinea)

cmake_minimum_required(VERSION 3.8)



add_library(guinea)

target_compile_features(guinea PUBLIC cxx_std_11)

if(EMSCRIPTEN)
    #target_link_options(guinea PRIVATE "-s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")# --shell-file ext/imgui/examples/example_emscripten/shell_minimal.html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGL2=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FULL_ES3=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${PROJECT_SOURCE_DIR}/ext/imgui/examples/example_emscripten/shell_minimal.html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    #target_link_libraries(guinea PUBLIC "-s USE_SDL=2 -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1")
    #set_target_properties(guinea PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1")
else()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake"
                    "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_add_remote(NAME bincrafters INDEX 1
                URL https://api.bintray.com/conan/bincrafters/public-conan)

    conan_cmake_run(CONANFILE conanfile.txt
                    BASIC_SETUP)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

target_include_directories(guinea PUBLIC
    "include"
)

option(GUINEA_BUILD_EXAMPLES "build all examples" ON)

add_subdirectory(ext)
add_subdirectory(src)
if(GUINEA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if(NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)

    target_include_directories(guinea PRIVATE ${OPENGL_INCLUDE_DIRS})

    target_link_libraries(guinea PRIVATE ${OPENGL_LIBRARIES} ${CONAN_LIBS})
endif()

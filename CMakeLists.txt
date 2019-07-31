project(guinea)

cmake_minimum_required(VERSION 3.7)

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

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

add_library(guinea)

target_include_directories(guinea PUBLIC
    "include"
)


option(GUINEA_BUILD_EXAMPLES "build all examples" ON)


add_subdirectory(ext)
add_subdirectory(src)
if(GUINEA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

find_package(OpenGL REQUIRED)

target_include_directories(guinea PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(guinea PRIVATE ${OPENGL_LIBRARIES} ${CONAN_LIBS})

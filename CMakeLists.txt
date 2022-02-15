cmake_minimum_required(VERSION 3.12)
project(guinea)

add_library(guinea STATIC)


add_library(guinea_base INTERFACE)
target_compile_features(guinea_base INTERFACE cxx_std_17)

add_library(guinea::guinea ALIAS guinea)


target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(guinea_base INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(MSVC)
  target_compile_definitions(guinea PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

option(BUILD_GUINEA_EXAMPLES "Build guinea examples" OFF)
option(BUILD_GUINEA_TESTS "Build guinea tests" OFF)
if (EMSCRIPTEN)
  option(BUILD_GUINEA_BACKEND_STATIC "Build guinea backend static" ON)
else()
  option(BUILD_GUINEA_BACKEND_STATIC "Build guinea backend static" OFF)
endif()

set(GUINEA_PLATFORM
    "sdl"
    CACHE STRING "Platform")

if(EMSCRIPTEN)
  set(GUINEA_RENDERER
      "opengl3"
      CACHE STRING "Renderer")
else()
  set(GUINEA_RENDERER
      "opengl2"
      CACHE STRING "Renderer")
endif()

find_package(SDL2 CONFIG)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)

include(GNUInstallDirs)

add_subdirectory(src)

target_link_libraries(guinea_base INTERFACE imgui)
target_link_libraries(guinea PUBLIC guinea_base implot imgui_ne imgui_md)

if (BUILD_GUINEA_BACKEND_STATIC)
  target_link_libraries(guinea PRIVATE guinea_${GUINEA_PLATFORM}_${GUINEA_RENDERER})
endif()

target_compile_definitions(guinea PUBLIC DEFAULT_GUINEA_BACKEND=\"guinea_${GUINEA_PLATFORM}_${GUINEA_RENDERER}\")

if(BUILD_GUINEA_EXAMPLES)
  add_subdirectory(examples)
endif()

install(
  TARGETS guinea guinea_base imgui implot imgui_ne imgui_md
  EXPORT guineaTargets
  RUNTIME #
          COMPONENT guinea_Runtime
  LIBRARY #
          COMPONENT guinea_Runtime NAMELINK_COMPONENT guinea_Development
  ARCHIVE #
          COMPONENT guinea_Development
  INCLUDES #
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Allow package maintainers to freely override the path for the configs
set(guinea_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_DATADIR}/guinea"
    CACHE PATH "CMake package config location relative to the install prefix")
mark_as_advanced(guinea_INSTALL_CMAKEDIR)

install(
  EXPORT guineaTargets
  NAMESPACE guinea::
  DESTINATION "${guinea_INSTALL_CMAKEDIR}"
  COMPONENT guinea_Development)

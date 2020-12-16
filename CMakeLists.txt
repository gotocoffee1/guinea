cmake_minimum_required(VERSION 3.12)
project(guinea)

add_library(guinea STATIC)

add_library(guinea::guinea ALIAS guinea)

target_compile_features(guinea PUBLIC cxx_std_17)

target_include_directories(
  guinea PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
  target_compile_definitions(guinea PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(guinea PUBLIC IMGUI_USER_CONFIG=\"config.hpp\")

set(GUINEA_PLATFORM
    "sdl"
    CACHE STRING "Platform")

if(EMSCRIPTEN)
  set(GUINEA_RENDERER
      "opengl3"
      CACHE STRING "Platform")
else()
  set(GUINEA_RENDERER
      "opengl2"
      CACHE STRING "Platform")
endif()

target_compile_definitions(guinea PUBLIC GUINEA_PLATFORM_${GUINEA_PLATFORM} GUINEA_RENDERER_${GUINEA_RENDERER})

if(GUINEA_PLATFORM MATCHES "sdl")
  if(EMSCRIPTEN)
    target_compile_options(guinea PUBLIC "SHELL:-s USE_SDL=2")
    target_link_options(guinea PUBLIC "SHELL:-s USE_SDL=2")
  else()

    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(guinea PRIVATE SDL2::SDL2)

    get_target_property(target_type SDL2::SDL2 TYPE)
    if(target_type STREQUAL SHARED_LIBRARY)
      # install( PROGRAMS $<TARGET_FILE:SDL2::SDL2> DESTINATION ${INSTALL_DIR}
      # COMPONENT guinea)
    endif()
  endif()
endif()

if(GUINEA_RENDERER MATCHES "opengl." AND NOT EMSCRIPTEN)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL REQUIRED)
  target_link_libraries(guinea PRIVATE ${OPENGL_LIBRARIES})
endif()

add_subdirectory(src)
add_subdirectory(examples)

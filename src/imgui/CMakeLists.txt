FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG 1ee252772ae9c0a971d06257bb5c89f628fa696a)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

add_library(imgui STATIC)

target_sources(
  imgui
  PRIVATE "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_tables.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")

target_include_directories(
  imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set(IMGUI_USER_CONFIG
    \"imgui_config.hpp\"
    CACHE STRING "Imgui config file")

target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG=${IMGUI_USER_CONFIG})

function(backend type name)
  set(tar imgui_${type}_${name})
  add_library(${tar} STATIC EXCLUDE_FROM_ALL
              "${imgui_SOURCE_DIR}/backends/imgui_impl_${name}.cpp")
  target_include_directories(
    ${tar} PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>)
  target_link_libraries(${tar} PUBLIC imgui)

  install(
    TARGETS ${tar}
    EXPORT guineaTargets
    OPTIONAL
    RUNTIME #
            COMPONENT guinea_Runtime
    LIBRARY #
            COMPONENT guinea_Runtime NAMELINK_COMPONENT guinea_Development
    ARCHIVE #
            COMPONENT guinea_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endfunction()

if(NOT EMSCRIPTEN)
  find_package(SDL2 CONFIG)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL)
endif()

if(OPENGL_FOUND OR EMSCRIPTEN)
  backend("renderer" opengl2)
  backend("renderer" opengl3)
  if(NOT EMSCRIPTEN)
    target_link_libraries(imgui_renderer_opengl2 PUBLIC ${OPENGL_LIBRARIES})
    target_link_libraries(imgui_renderer_opengl3 PUBLIC ${OPENGL_LIBRARIES})
  endif()
endif()

if(TARGET SDL2::SDL2 OR EMSCRIPTEN)
  backend("platform" sdl)

  if(EMSCRIPTEN)
    target_compile_options(imgui_platform_sdl PUBLIC "SHELL:-s USE_SDL=2")
    target_link_options(imgui_platform_sdl PUBLIC "SHELL:-s USE_SDL=2")
  else()
    target_link_libraries(imgui_platform_sdl PUBLIC SDL2::SDL2)
  endif()
endif()

if(WIN32)
  backend("renderer" dx11)
  backend("platform" win32)
endif()

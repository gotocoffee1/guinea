FetchContent_Populate(
  imgui
  QUIET
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG adfa5364cd84db127f8f5d093be90c2e3608be0f)

add_library(imgui STATIC)

target_sources(
  imgui
  PRIVATE "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_tables.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
          "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
          "${CMAKE_CURRENT_SOURCE_DIR}/guinea_config.cpp")

target_include_directories(
  imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set(IMGUI_USER_CONFIG
    \"guinea_config.hpp\"
    CACHE STRING "Imgui config file")

target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG=${IMGUI_USER_CONFIG})

find_package(Freetype)

if(TARGET Freetype::Freetype)
  target_compile_definitions(imgui PRIVATE IMGUI_ENABLE_FREETYPE)
  target_link_libraries(imgui PRIVATE Freetype::Freetype)
  target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp")
endif()

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
  if(NOT WIN32)
    find_package(X11)
  endif()
endif()

if(OPENGL_FOUND OR EMSCRIPTEN)
  backend("renderer" opengl2)
  backend("renderer" opengl3)
  if(NOT EMSCRIPTEN)
    target_link_libraries(imgui_renderer_opengl2 PUBLIC ${OPENGL_LIBRARIES})
    target_link_libraries(imgui_renderer_opengl3 PUBLIC ${OPENGL_LIBRARIES})
    target_include_directories(imgui_renderer_opengl2 PUBLIC ${OPENGL_INCLUDE_DIR})
    target_include_directories(imgui_renderer_opengl3 PUBLIC ${OPENGL_INCLUDE_DIR})
  endif()
endif()

if(TARGET SDL2::SDL2 OR EMSCRIPTEN)
  backend("platform" sdl2)

  if(EMSCRIPTEN)
    target_compile_options(imgui_platform_sdl2 PUBLIC "SHELL:-s USE_SDL=2")
    target_link_options(imgui_platform_sdl2 PUBLIC "SHELL:-s USE_SDL=2")
  else()
    target_link_libraries(imgui_platform_sdl2 PUBLIC SDL2::SDL2)
    if(NOT WIN32)
      target_link_libraries(imgui_platform_sdl2 PUBLIC ${X11_LIBRARIES})
      target_include_directories(imgui_platform_sdl2 PUBLIC ${X11_INCLUDE_DIR})
    endif()
  endif()
endif()

if(WIN32)
  backend("renderer" dx11)
  backend("platform" win32)
endif()

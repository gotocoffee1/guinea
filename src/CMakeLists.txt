include(FetchContent)

add_subdirectory(imgui)
add_subdirectory(club)
add_subdirectory(md)
add_subdirectory(ne)
add_subdirectory(plot)

target_sources(guinea PRIVATE "${CMAKE_CURRENT_LIST_DIR}/guinea.cpp"
                              "${CMAKE_CURRENT_LIST_DIR}/widgets.cpp")

function(lib platform renderer)
  if(TARGET imgui_platform_${platform} AND TARGET imgui_renderer_${renderer})
    if(EMSCRIPTEN)
      set(extra _emscripten)
    endif()
    set(tar guinea_${platform}_${renderer})
    MESSAGE(STATUS ${tar})
    if(BUILD_GUINEA_BACKEND_STATIC)
      add_library(${tar} STATIC)
      target_compile_definitions(${tar} PUBLIC BUILD_GUINEA_BACKEND_STATIC)
    else()
      add_library(${tar} SHARED)
    endif()
    target_sources(${tar} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${tar}${extra}.cpp)
    target_link_libraries(${tar} PRIVATE imgui_platform_${platform}
                                         imgui_renderer_${renderer} guinea_base)

    install(
      TARGETS ${tar}
      EXPORT guineaTargets
      RUNTIME #
              COMPONENT guinea_Runtime
      LIBRARY #
              COMPONENT guinea_Runtime NAMELINK_COMPONENT guinea_Development
      ARCHIVE #
              COMPONENT guinea_Development
      INCLUDES #
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
  endif()
endfunction()

lib(sdl opengl3)

if(NOT EMSCRIPTEN)
  lib(sdl opengl2)
  lib(sdl dx11)
  lib(sdl dx12)
endif()

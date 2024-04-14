include(FetchContent)

add_subdirectory(imgui)
add_subdirectory(club)
add_subdirectory(md)
add_subdirectory(ne)
add_subdirectory(plot)

target_sources(guinea PRIVATE "${CMAKE_CURRENT_LIST_DIR}/guinea.cpp"
                              "${CMAKE_CURRENT_LIST_DIR}/widgets.cpp")

set(GUINEA_ALL_BACKENDS
    ""
    CACHE INTERNAL "all guinea backends")

function(guinea_backend platform renderer)
  if(TARGET imgui_platform_${platform} AND TARGET imgui_renderer_${renderer})
    set(tar guinea_${platform}_${renderer})
    if(BUILD_GUINEA_BACKEND_STATIC)
      add_library(${tar} STATIC EXCLUDE_FROM_ALL )
      target_compile_definitions(${tar} PUBLIC BUILD_GUINEA_BACKEND_STATIC)
    else()
      add_library(${tar} MODULE EXCLUDE_FROM_ALL )
    endif()
    add_library(guinea::${platform}_${renderer} ALIAS ${tar})
    target_sources(${tar} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${tar}.cpp)
    target_link_libraries(${tar} PRIVATE imgui_platform_${platform}
                                         imgui_renderer_${renderer} guinea_base)

    set(GUINEA_ALL_BACKENDS
        "guinea::${platform}_${renderer};${GUINEA_ALL_BACKENDS}"
        CACHE INTERNAL "all guinea backends")
    install(
      TARGETS ${tar}
      EXPORT guineaTargets
      OPTIONAL
      RUNTIME #
              COMPONENT guinea_Runtime
      LIBRARY #
              COMPONENT guinea_Runtime
              NAMELINK_COMPONENT guinea_Development
              DESTINATION ${CMAKE_INSTALL_BINDIR}
      ARCHIVE #
              COMPONENT guinea_Development
      INCLUDES #
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
  endif()
endfunction()

guinea_backend(sdl2 opengl3)

if(NOT EMSCRIPTEN)
  guinea_backend(sdl2 opengl2)
  guinea_backend(sdl2 dx11)
  guinea_backend(win32 dx11)
endif()

message(STATUS "Available backends: ${GUINEA_ALL_BACKENDS}")

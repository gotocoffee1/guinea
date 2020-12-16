if(EMSCRIPTEN)
  target_sources(
    guinea
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/guinea_${GUINEA_PLATFORM}_${GUINEA_RENDERER}_emscripten.cpp"
  )
else()
  target_sources(
    guinea
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/guinea_${GUINEA_PLATFORM}_${GUINEA_RENDERER}.cpp"
  )
endif()

target_sources(guinea PRIVATE "${CMAKE_CURRENT_LIST_DIR}/guinea.cpp"
                              "${CMAKE_CURRENT_LIST_DIR}/widgets.cpp")

include(FetchContent)

add_subdirectory(club)
add_subdirectory(imgui)
add_subdirectory(md)
add_subdirectory(ne)
add_subdirectory(plot)

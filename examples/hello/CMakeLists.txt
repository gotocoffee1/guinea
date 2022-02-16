add_executable(example main.cpp)
target_link_libraries(example guinea::guinea)

if(EMSCRIPTEN)
  set_target_properties(example PROPERTIES SUFFIX ".html")
endif()

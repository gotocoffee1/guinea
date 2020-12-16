add_executable(example main.cpp)
target_link_libraries(example guinea::guinea)

if(EMSCRIPTEN)
  target_compile_options(example PRIVATE "SHELL:-s WASM=1")
  target_link_options(example PRIVATE "SHELL:-s WASM=1")
  set_target_properties(example PROPERTIES SUFFIX ".html")
endif()


target_sources(guinea PRIVATE 
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/examples/imgui_impl_sdl.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/imgui/examples/libs/gl3w/GL/gl3w.c"
)

target_include_directories(guinea PRIVATE 
    "imgui/examples"
    "imgui/examples/libs/gl3w"
)

target_include_directories(guinea PUBLIC 
    "imgui"
)

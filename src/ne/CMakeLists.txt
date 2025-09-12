FetchContent_Populate(
  imgui-node-editor
  QUIET
  GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
  GIT_TAG b302971455b3719ec9b5fb94b2f92d27c62b9ff0)

add_library(imgui_ne STATIC)

target_include_directories(imgui_ne PUBLIC $<BUILD_INTERFACE:${imgui-node-editor_SOURCE_DIR}>)
target_sources(
  imgui_ne
  PRIVATE "${imgui-node-editor_SOURCE_DIR}/crude_json.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_canvas.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_api.cpp")

target_compile_definitions(imgui_ne PRIVATE _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)
target_link_libraries(imgui_ne PUBLIC imgui)

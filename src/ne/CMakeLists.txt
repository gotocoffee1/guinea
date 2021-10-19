FetchContent_Declare(
  imgui-node-editor
  GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
  GIT_TAG 687a72f940c76cf5064e13fe55fa0408c18fcbe4)

FetchContent_GetProperties(imgui-node-editor)
if(NOT imgui-node-editor_POPULATED)
  FetchContent_Populate(imgui-node-editor)

endif()

target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui-node-editor_SOURCE_DIR}>)
target_sources(
  guinea
  PRIVATE "${imgui-node-editor_SOURCE_DIR}/crude_json.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_canvas.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_api.cpp")

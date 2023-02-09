FetchContent_Declare(
  imgui-node-editor
  GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
  GIT_TAG 99ec923a39062f556ec7676fb9ba6d302d109f76)

FetchContent_GetProperties(imgui-node-editor)
if(NOT imgui-node-editor_POPULATED)
  FetchContent_Populate(imgui-node-editor)
endif()

add_library(imgui_ne STATIC)

target_include_directories(imgui_ne PUBLIC $<BUILD_INTERFACE:${imgui-node-editor_SOURCE_DIR}>)
target_sources(
  imgui_ne
  PRIVATE "${imgui-node-editor_SOURCE_DIR}/crude_json.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_canvas.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor.cpp"
          "${imgui-node-editor_SOURCE_DIR}/imgui_node_editor_api.cpp")

target_link_libraries(imgui_ne PUBLIC imgui)

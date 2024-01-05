FetchContent_Declare(
  imgui_markdown
  GIT_REPOSITORY https://github.com/juliettef/imgui_markdown.git
  GIT_TAG 61a181bdb83f450f852f7cf5d1282d8cda1c0f57)

FetchContent_GetProperties(imgui_markdown)
if(NOT imgui_markdown_POPULATED)
  FetchContent_Populate(imgui_markdown)
endif()

add_library(imgui_md INTERFACE)

target_include_directories(imgui_md INTERFACE $<BUILD_INTERFACE:${imgui_markdown_SOURCE_DIR}>)

target_link_libraries(imgui_md INTERFACE imgui)

FetchContent_Declare(
  imgui_markdown
  GIT_REPOSITORY https://github.com/juliettef/imgui_markdown.git
  GIT_TAG 0301a309a1692f1e8f7b8eb0f58d2fca5fbfc610)

FetchContent_GetProperties(imgui_markdown)
if(NOT imgui_markdown_POPULATED)
  FetchContent_Populate(imgui_markdown)
endif()
target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_markdown_SOURCE_DIR}>)

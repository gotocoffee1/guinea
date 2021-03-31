FetchContent_Declare(
  imgui_markdown
  GIT_REPOSITORY https://github.com/juliettef/imgui_markdown.git
  GIT_TAG 18657eb4b8d1805507afe7f197321b076b6edc19)

FetchContent_GetProperties(imgui_markdown)
if(NOT imgui_markdown_POPULATED)
  FetchContent_Populate(imgui_markdown)
endif()
target_include_directories(guinea PUBLIC "${imgui_markdown_SOURCE_DIR}")

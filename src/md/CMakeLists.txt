FetchContent_Declare(
  imgui_markdown
  GIT_REPOSITORY https://github.com/juliettef/imgui_markdown.git
  GIT_TAG 7c8e8272938b721324e90f1d1ccecc50c73b7723)

FetchContent_GetProperties(imgui_markdown)
if(NOT imgui_markdown_POPULATED)
  FetchContent_Populate(imgui_markdown)
endif()
target_include_directories(guinea PUBLIC "${imgui_markdown_SOURCE_DIR}")

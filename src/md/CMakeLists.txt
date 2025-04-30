FetchContent_Populate(
  imgui_md
  QUIET
  GIT_REPOSITORY https://github.com/gotocoffee1/imgui_md.git
  GIT_TAG d4986daa87880c92016d85963e7f4195007a8157)

FetchContent_Populate(
  md4c
  QUIET
  GIT_REPOSITORY https://github.com/mity/md4c.git
  GIT_TAG 481fbfbdf72daab2912380d62bb5f2187d438408) 

add_library(imgui_md STATIC)

target_include_directories(imgui_md PUBLIC $<BUILD_INTERFACE:${imgui_md_SOURCE_DIR}> $<BUILD_INTERFACE:${md4c_SOURCE_DIR}/src>)

target_link_libraries(imgui_md PUBLIC imgui)
target_sources(imgui_md PRIVATE ${imgui_md_SOURCE_DIR}/imgui_md.cpp ${md4c_SOURCE_DIR}/src/md4c.c)

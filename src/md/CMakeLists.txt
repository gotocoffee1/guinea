FetchContent_Declare(
  imgui_md
  GIT_REPOSITORY https://github.com/mekhontsev/imgui_md.git
  GIT_TAG b9f86beda0cd657b4d07bd9ce88dd05c9088384f
)

FetchContent_Declare(
  md4c 
  GIT_REPOSITORY https://github.com/mity/md4c.git
  GIT_TAG 481fbfbdf72daab2912380d62bb5f2187d438408
)

FetchContent_GetProperties(imgui_md)
if(NOT imgui_md_POPULATED)
  FetchContent_Populate(imgui_md)
endif()

FetchContent_GetProperties(md4c)
if(NOT md4c_POPULATED)
  FetchContent_Populate(md4c)
endif()

add_library(imgui_md STATIC)

target_include_directories(imgui_md PUBLIC $<BUILD_INTERFACE:${imgui_md_SOURCE_DIR}> $<BUILD_INTERFACE:${md4c_SOURCE_DIR}/src>)

target_link_libraries(imgui_md PUBLIC imgui)
target_sources(imgui_md PRIVATE ${imgui_md_SOURCE_DIR}/imgui_md.cpp ${md4c_SOURCE_DIR}/src/md4c.c)

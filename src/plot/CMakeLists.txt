FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG 419a8a0f5fcb77e1e7c19ab540441686bfe21bca)

FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()

add_library(implot STATIC)


target_sources(implot PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp"
                              "${implot_SOURCE_DIR}/implot_demo.cpp")
target_include_directories(implot PUBLIC $<BUILD_INTERFACE:${implot_SOURCE_DIR}>)

target_link_libraries(implot PUBLIC imgui)


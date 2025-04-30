FetchContent_Populate(
  implot
  QUIET
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG 3da8bd34299965d3b0ab124df743fe3e076fa222)


add_library(implot STATIC)


target_sources(implot PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp"
                              "${implot_SOURCE_DIR}/implot_demo.cpp")
target_include_directories(implot PUBLIC $<BUILD_INTERFACE:${implot_SOURCE_DIR}>)

target_link_libraries(implot PUBLIC imgui)


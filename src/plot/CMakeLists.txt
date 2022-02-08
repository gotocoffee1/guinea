FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG b47c8bacdbc78bc521691f70666f13924bb522ab)

FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()
target_sources(guinea PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp")
target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${implot_SOURCE_DIR}>)

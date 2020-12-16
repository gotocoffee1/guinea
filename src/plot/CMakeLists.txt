FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG a9d334791563cdaf9bd0bf7f9899a67bcd03179b)

FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()
target_sources(guinea PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp")
target_include_directories(guinea PUBLIC "${implot_SOURCE_DIR}")

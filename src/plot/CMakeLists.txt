FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG 14d4c96d0c169198e60079d467111139b47242f5)

FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()
target_sources(guinea PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp")
target_include_directories(guinea PUBLIC "${implot_SOURCE_DIR}")

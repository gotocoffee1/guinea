FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG e64df657b26f3a11237e15f61834a59d8ba069c0)

FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()
target_sources(guinea PRIVATE "${implot_SOURCE_DIR}/implot.cpp"
                              "${implot_SOURCE_DIR}/implot_items.cpp")
target_include_directories(guinea PUBLIC "${implot_SOURCE_DIR}")

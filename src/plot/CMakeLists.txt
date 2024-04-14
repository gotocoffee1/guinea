FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG f156599faefe316f7dd20fe6c783bf87c8bb6fd9)

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


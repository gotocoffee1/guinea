FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG 1ee252772ae9c0a971d06257bb5c89f628fa696a)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

target_sources(
  guinea
  PRIVATE "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_tables.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")

target_sources(
  guinea
  PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_${GUINEA_PLATFORM}.cpp"
          "${imgui_SOURCE_DIR}/backends/imgui_impl_${GUINEA_RENDERER}.cpp")

target_include_directories(guinea PRIVATE $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>)

target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>)

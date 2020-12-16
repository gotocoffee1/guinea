FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG e3ac52630a727ddaa5d642cba8b57d0b166e9fd0)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

target_sources(
  guinea
  PRIVATE "${imgui_SOURCE_DIR}/imgui.cpp"
          "${imgui_SOURCE_DIR}/imgui_draw.cpp"
          "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
          "${imgui_SOURCE_DIR}/imgui_demo.cpp"
          "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")

target_sources(
  guinea
  PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_${GUINEA_PLATFORM}.cpp"
          "${imgui_SOURCE_DIR}/backends/imgui_impl_${GUINEA_RENDERER}.cpp")

target_include_directories(guinea PRIVATE "${imgui_SOURCE_DIR}/backends")

target_include_directories(guinea PUBLIC "${imgui_SOURCE_DIR}")

FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG d4cd9896e15a03e92702a578586c3f91bbde01e8)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_club_SOURCE_DIR}>)
endif()

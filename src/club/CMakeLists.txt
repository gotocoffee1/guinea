FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG ea49dd3c6803088d50b496e3fe981501543b7cbc)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_club_SOURCE_DIR}>)
endif()

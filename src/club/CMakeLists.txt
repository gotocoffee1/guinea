FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG 60275e79c3e0b0568ee1e41486233e958eac2e80)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC "${imgui_club_SOURCE_DIR}")
endif()

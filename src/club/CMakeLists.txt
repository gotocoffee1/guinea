FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG 5e5f32c66ccd486891d46f66ce3a5bbcb9d769fc)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC "${imgui_club_SOURCE_DIR}")
endif()

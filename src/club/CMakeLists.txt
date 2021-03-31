FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG 02e679b7f4cfb01f9480dcbcac59552879f96573)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC "${imgui_club_SOURCE_DIR}")
endif()

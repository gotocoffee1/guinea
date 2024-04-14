FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG d398d908c5494f14301739dd2975f4b19258eb15)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_club_SOURCE_DIR}>)
endif()

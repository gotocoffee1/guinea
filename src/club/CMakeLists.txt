FetchContent_Declare(
  imgui_club
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG 17510b03602c8b6c7ed48fd8fa29d9a75eb10dc6)

FetchContent_GetProperties(imgui_club)
if(NOT imgui_club_POPULATED)
  FetchContent_Populate(imgui_club)
  target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_club_SOURCE_DIR}>)
endif()

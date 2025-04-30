FetchContent_Populate(
  imgui_club
  QUIET
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG b89dbdd2154622cce958d460c360bb9db644d4db)

target_include_directories(guinea PUBLIC $<BUILD_INTERFACE:${imgui_club_SOURCE_DIR}>)

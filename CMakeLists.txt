project(guinea VERSION 0.1.0)

add_library(guinea STATIC)

add_library(guinea_base INTERFACE)
target_compile_features(guinea_base INTERFACE cxx_std_17)

add_library(guinea::guinea ALIAS guinea)

target_include_directories(
  guinea PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(
  guinea_base INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(MSVC)
  target_compile_definitions(guinea PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

option(BUILD_GUINEA_EXAMPLES "Build guinea examples" OFF)
option(BUILD_GUINEA_TESTS "Build guinea tests" OFF)

if(EMSCRIPTEN)
  set(INIT_BUILD_GUINEA_BACKEND_STATIC ON)
else()
  set(INIT_BUILD_GUINEA_BACKEND_STATIC OFF)
endif()

if(WIN32)
  set(INIT_GUINEA_PLATFORM "win32")
else()
  set(INIT_GUINEA_PLATFORM "sdl2")
endif()

if(EMSCRIPTEN)
  set(INIT_GUINEA_RENDERER "opengl3")
elseif(WIN32)
  set(INIT_GUINEA_RENDERER "dx11")
else()
  set(INIT_GUINEA_RENDERER "opengl2")
endif()

option(BUILD_GUINEA_BACKEND_STATIC "Build guinea backend static"
       ${INIT_BUILD_GUINEA_BACKEND_STATIC})

set(GUINEA_RENDERER
    ${INIT_GUINEA_RENDERER}
    CACHE STRING "Renderer")
set(GUINEA_PLATFORM
    ${INIT_GUINEA_PLATFORM}
    CACHE STRING "Platform")

set(DEFAULT_GUINEA_BACKEND
    "guinea_${GUINEA_PLATFORM}_${GUINEA_RENDERER}"
    CACHE INTERNAL "default backend")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_subdirectory(src)

target_link_libraries(guinea_base INTERFACE imgui)
target_link_libraries(guinea PUBLIC guinea_base implot imgui_ne imgui_md)

if(BUILD_GUINEA_BACKEND_STATIC)
  target_link_libraries(guinea PRIVATE ${DEFAULT_GUINEA_BACKEND})
else()
  add_dependencies(guinea ${DEFAULT_GUINEA_BACKEND})
endif()

target_compile_definitions(
  guinea PUBLIC DEFAULT_GUINEA_BACKEND=\"${DEFAULT_GUINEA_BACKEND}\")

if(BUILD_GUINEA_EXAMPLES)
  add_subdirectory(examples)
endif()

set(package guinea)

install(
  TARGETS guinea guinea_base imgui implot imgui_ne imgui_md
  EXPORT guineaTargets
  RUNTIME #
          COMPONENT guinea_Runtime
  LIBRARY #
          COMPONENT guinea_Runtime NAMELINK_COMPONENT guinea_Development
  ARCHIVE #
          COMPONENT guinea_Development
  INCLUDES #
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

write_basic_package_version_file("${package}ConfigVersion.cmake"
                                 COMPATIBILITY SameMajorVersion)

# Allow package maintainers to freely override the path for the configs
set(guinea_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix")
mark_as_advanced(guinea_INSTALL_CMAKEDIR)

install(
  FILES cmake/install-config.cmake
  DESTINATION "${guinea_INSTALL_CMAKEDIR}"
  RENAME "${package}Config.cmake"
  COMPONENT guinea_Development)

install(
  FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
  DESTINATION "${guinea_INSTALL_CMAKEDIR}"
  COMPONENT guinea_Development)

install(
  EXPORT guineaTargets
  NAMESPACE guinea::
  DESTINATION "${guinea_INSTALL_CMAKEDIR}"
  COMPONENT guinea_Development)

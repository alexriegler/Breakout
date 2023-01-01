cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    Breakout
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    Breakout_lib
    OBJECT
        "src/game.h"
        "src/game.cpp"
        "src/shader.h"
        "src/shader.cpp"
        "src/texture.h"
        "src/texture.cpp"
        "src/resource_manager.h"
        "src/resource_manager.cpp"
)

target_include_directories(
    Breakout_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(Breakout_lib PUBLIC cxx_std_17)

# 3rd party libraries
find_package(glad CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glad::glad)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glfw)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glm::glm)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(Breakout_lib PRIVATE ${STB_INCLUDE_DIRS})

# ---- Declare executable ----

add_executable(Breakout_exe src/main.cpp)
add_executable(Breakout::exe ALIAS Breakout_exe)

set_property(TARGET Breakout_exe PROPERTY OUTPUT_NAME Breakout)

target_compile_features(Breakout_exe PRIVATE cxx_std_17)

target_link_libraries(Breakout_exe PRIVATE Breakout_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT Breakout_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of Breakout"
  )
endif()

include(cmake/dev-mode.cmake)

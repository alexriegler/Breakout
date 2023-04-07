cmake_minimum_required(VERSION 3.25)

include(cmake/prelude.cmake)

project(
    Breakout
    VERSION 1.0.0
    DESCRIPTION "A simple Breakout clone"
    HOMEPAGE_URL "https://github.com/alexriegler/Breakout"
    LANGUAGES CXX
)

# ---- CMake configuration ----

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(BUILD_SHARED_LIBS "OFF")

# ---- C++ options ----

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
set(CMAKE_CXX_EXTENSIONS "OFF")

# ---- Options ----

option(DISABLE_AUDIO "Build ${PROJECT_NAME} without the ability to output audio" OFF)

# ---- Compiler options ----

add_library(Breakout_compiler_flags INTERFACE)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(Breakout_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic;-Wconversion;-Wsign-conversion;-Wcast-qual;-Wformat=2;-Wundef;-Werror=float-equal;-Wshadow;-Wcast-align;-Wunused;-Wnull-dereference;-Wdouble-promotion;-Wimplicit-fallthrough;-Wextra-semi;-Woverloaded-virtual;-Wnon-virtual-dtor;-Wold-style-cast>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-utf-8;-w14165;-w44242;-w44254;-w44263;-w34265;-w34287;-w44296;-w44365;-w44388;-w44464;-w14545;-w14546;-w14547;-w14549;-w14555;-w34619;-w34640;-w24826;-w14905;-w14906;-w14928;-w45038;-W4;-permissive-;-volatile:iso;-Zc:preprocessor;-Zc:__cplusplus;-Zc:externConstexpr;-Zc:throwingNew;-EHsc>>"
)

# ---- Declare library ----

add_library(
    Breakout_lib
    OBJECT
        "src/game.h" "src/game.cpp"
        "src/shader.h" "src/shader.cpp"
        "src/texture.h" "src/texture.cpp"
        "src/resource_manager.h" "src/resource_manager.cpp"
        "src/sprite_renderer.h" "src/sprite_renderer.cpp"
        "src/game_object.h" "src/game_object.cpp"
        "src/game_level.h" "src/game_level.cpp"
        "src/ball_object.h" "src/ball_object.cpp"
        "src/particle_generator.h" "src/particle_generator.cpp"
        "src/post_processor.h" "src/post_processor.cpp"
        "src/power_up.h"
        "src/sound_engine.h" "src/sound_engine.cpp"
        "src/text_renderer.h" "src/text_renderer.cpp")

target_include_directories(
    Breakout_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

if (DISABLE_AUDIO)
    target_compile_definitions(Breakout_lib PRIVATE DISABLE_AUDIO=1)
endif()

target_link_libraries(Breakout_lib PUBLIC Breakout_compiler_flags)

# ---- 3rd party libraries ----

find_package(glad CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC glm::glm)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(Breakout_lib PRIVATE ${STB_INCLUDE_DIRS})

if (NOT DISABLE_AUDIO)
    include(FetchContent)
    set(SFML_BUILD_AUDIO    ON)
    set(SFML_BUILD_GRAPHICS OFF)
    set(SFML_BUILD_WINDOW   OFF)
    set(SFML_BUILD_NETWORK  OFF)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        SYSTEM
    )
    FetchContent_MakeAvailable(SFML)
    target_link_libraries(Breakout_lib PUBLIC sfml-audio)
endif()

find_package(freetype CONFIG REQUIRED)
target_link_libraries(Breakout_lib PUBLIC freetype)

# ---- Declare executable ----

add_executable(Breakout_exe "src/main.cpp")
add_executable(Breakout::exe ALIAS Breakout_exe)

set_property(TARGET Breakout_exe PROPERTY OUTPUT_NAME Breakout)

target_link_libraries(Breakout_exe PRIVATE Breakout_lib)

# ---- Copy resources ----

add_custom_command(
    TARGET Breakout_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:Breakout_exe>/shaders)

add_custom_command(
    TARGET Breakout_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/textures
            $<TARGET_FILE_DIR:Breakout_exe>/textures)

add_custom_command(
    TARGET Breakout_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/levels
            $<TARGET_FILE_DIR:Breakout_exe>/levels)

add_custom_command(
    TARGET Breakout_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/audio
            $<TARGET_FILE_DIR:Breakout_exe>/audio)

add_custom_command(
    TARGET Breakout_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fonts
            $<TARGET_FILE_DIR:Breakout_exe>/fonts)

# ---- Copy dependencies (.dll) ----

if (WIN32 AND NOT DISABLE_AUDIO)
    add_custom_command(
        TARGET Breakout_exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll"
        $<TARGET_FILE_DIR:Breakout_exe>)
endif()

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

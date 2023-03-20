# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(BreakoutTests LANGUAGES CXX)

# ---- C++ options ----

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
set(CMAKE_CXX_EXTENSIONS "OFF")

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(Breakout_test src/Breakout_test.cpp)
target_link_libraries(
    Breakout_test PRIVATE
    Breakout_lib
    Catch2::Catch2WithMain
)

# ---- Copy dependencies (.dll) ----

if (WIN32 AND NOT DISABLE_AUDIO)
    add_custom_command(
        TARGET Breakout_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll"
        $<TARGET_FILE_DIR:Breakout_test>)
endif()

# ---- Register tests ----

catch_discover_tests(Breakout_test)

# ---- End-of-file commands ----

add_folders(Test)

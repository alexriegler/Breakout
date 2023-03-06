# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(BreakoutTests LANGUAGES CXX)

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
target_compile_features(Breakout_test PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET Breakout_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${irrKlang_BINARY_DIR}/irrKlang.dll"
                "${irrKlang_BINARY_DIR}/ikpMP3.dll"
                "$<TARGET_FILE_DIR:Breakout_test>")
endif()

catch_discover_tests(Breakout_test)

# ---- End-of-file commands ----

add_folders(Test)

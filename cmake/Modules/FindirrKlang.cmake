# - Try to find irrKlang
# Once done this will define
#  irrKlang_FOUND        - System has irrKlang
#  irrKlang_INCLUDE_DIRS - The irrKlang include directories
#  irrKlang_LIBRARIES    - The libraries needed to use irrKlang
#  irrKlang_BINARY_DIR   - The directory where irrKlang's binary libraries (.dll,
#                          .so, .dylib) are stored
#  irrKlang_BINARY_DIR   - The directory where irrKlang's library information (.lib, .exp)
#                          is stored

set(irrKlang_PATH_SUFFIX "irrKlang-64bit-1.6.0")
set(irrKlang_BINARY_DIR "${CMAKE_PREFIX_PATH}/${irrKlang_PATH_SUFFIX}/bin")
set(irrKlang_LIBRARY_DIR "${CMAKE_PREFIX_PATH}/${irrKlang_PATH_SUFFIX}/lib")

find_path(
    irrKlang_INCLUDE_DIR
    NAMES irrKlang.h
    HINTS "${CMAKE_PREFIX_PATH}/${irrKlang_PATH_SUFFIX}/include")

find_library(
    irrKlang_LIBRARY
    NAMES irrKlang irrklang IrrKlang
    HINTS
        "${irrKlang_LIBRARY_DIR}"
        "${irrKlang_BINARY_DIR}")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set irrKlang_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(irrKlang  DEFAULT_MSG
                                  irrKlang_LIBRARY irrKlang_INCLUDE_DIR)

mark_as_advanced(irrKlang_INCLUDE_DIR irrKlang_LIBRARY)

set(irrKlang_LIBRARIES ${irrKlang_LIBRARY})
set(irrKlang_INCLUDE_DIRS ${irrKlang_INCLUDE_DIR})

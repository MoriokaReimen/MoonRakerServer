cmake_minimum_required(VERSION 2.8)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")



project(Math3D)
include_directories(include)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fPIC")
# math Library
add_library(math3d_static STATIC
src/Degree.cpp
src/Vector3.cpp
src/Quaternion.cpp
src/RotateFunc.cpp
)

enable_testing()
add_subdirectory(test)

# doxygen config
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND doxygen Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST__DIR}
        COMMENT "Generation documentation with doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# cpack config
install(TARGETS math3d_static
  RUNTIME DESTINATION "bin"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib")

install(DIRECTORY include/
  DESTINATION "include/Math3D"
  FILES_MATCHING PATTERN "*.hpp")

set(CPACK_OUTPUT_FILE_PREFIX "packages")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "MoriokaReimen")
include(CPack)

cmake_minimum_required(VERSION 2.8)
project(IMU)

INCLUDE_DIRECTORIES("include")
set(CMAKE_LIBRARY_PATH "/usr/local/lib" "lib")

# specify output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")

# enable C++11
set(CMAKE_CXX_COMPILER "g++-4.9")
set(CMAKE_C_COMPILER "g++-4.9")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_C_FLAGS "-std=c++11 -Wall")

set(SRC src/IMU.cpp)
add_library(imu_static ${SRC})
target_link_libraries(imu_static phidget21 math3d_static)

add_executable(test-imu src/test-imu.cpp ${SRC})
target_link_libraries(test-imu phidget21 math3d_static imu_static)

# doxygen config
find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(doc
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generate documentation with doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# cpack config
install(TARGETS imu_static
  RUNTIME DESTINATION "bin"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib")

install(DIRECTORY include
  DESTINATION "include/IMU"
  FILES_MATCHING PATTERN "*.hpp")

set(CPACK_OUTPUT_FILE_PREFIX "packages")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "MoriokaReimen")
include(CPack)

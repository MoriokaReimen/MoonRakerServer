set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fPIC")
add_executable(test_math
    test_main.cpp
    TestQuaternion.cpp
    TestVector3.cpp
    TestDegree.cpp
    TestRotateFunc.cpp
)
target_link_libraries(test_math
    math3d_static
    libgtest
    libgtest_main
    pthread
)

add_test(test_math ${CMAKE_BINARY_DIR}/bin/test_math)

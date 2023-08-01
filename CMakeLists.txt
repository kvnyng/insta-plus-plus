cmake_minimum_required(VERSION 3.22)
project(oners_demo)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")
set(CMAKE_LIBRARY_PATH "include")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

# add_subdirectory("${CMAKE_LIBRARY_PATH}" "${PROJECT_SOURCE_DIR}/build")
message(STATUS "${CMAKE_LIBRARY_PATH}")
add_executable(main main.cpp)
target_link_libraries(main PRIVATE CameraSDK)
#add_executable(camera_test examples/cameraSDKTest.cc)
#add_executable(connect examples/connect.cc)
#add_executable(recording examples/recording.cc)

#target_link_libraries(camera_test CameraSDK udev)
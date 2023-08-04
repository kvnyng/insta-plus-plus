cmake_minimum_required(VERSION 3.22)
project(insta-plus-plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")

include_directories(../../../include)

link_directories(../../../lib)

add_executable(main_child ../../archive/main.cpp)
add_executable(connect connect.cc)
add_executable(recording recording.cc)
add_executable(camera_test cameraSDKTest.cc)
add_executable(stream stream.cc)

# Add CameraSDK library
target_link_libraries(connect CameraSDK udev)
target_link_libraries(recording CameraSDK udev)
target_link_libraries(camera_test CameraSDK udev)
target_link_libraries(stream CameraSDK udev)

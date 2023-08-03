cmake_minimum_required(VERSION 3.22)
project(insta-plus-plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")

include_directories(../include)

link_directories(../lib)

add_executable(main_child main.cpp)
add_executable(connect connect.cc)
add_executable(recording recording.cc)
add_executable(camera_test cameraSDKTest.cc)

target_link_libraries(camera_test CameraSDK udev)
target_link_libraries(connect CameraSDK udev)
#add_executable(connect examples/connect.cc)
#add_executable(recording examples/recording.cc)

#target_link_libraries(camera_test CameraSDK udev)

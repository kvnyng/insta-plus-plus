cmake_minimum_required(VERSION 3.22)
project(insta-plus-plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")

include_directories(../../../include)

link_directories(../../../lib)

add_executable(camcontrol CameraController.cpp)

# Add CameraSDK library
target_link_libraries(camcontrol CameraSDK udev)

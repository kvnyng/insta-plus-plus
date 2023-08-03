cmake_minimum_required(VERSION 3.22)
project(insta-plus-plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")

include_directories(../include)

link_directories(../lib)

add_executable(p_stream programatic_stream.cc)
target_link_libraries(p_stream CameraSDK udev)

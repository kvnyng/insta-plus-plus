cmake_minimum_required(VERSION 3.22)
project(insta-plus-plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")

include_directories(../include)

find_package(Boost)
include_directories( ${Boost_INCLUDE_DIR} )
link_directories(../lib)
add_executable(CameraController CameraController.h CameraController.cpp)
target_link_libraries(CameraController LINK_PUBLIC ${Boost_LIBRARIES} CameraSDK udev)

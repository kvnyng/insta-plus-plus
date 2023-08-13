cmake_minimum_required(VERSION 3.22)
project(insta_plus_plus)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR "src")
set(CMAKE_LIBRARY_PATH "include")

SET(INSTA_PLUS_PLUS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/src" CACHE INTERNAL "")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

# find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
# find_library(AVCODEC_LIBRARY avcodec)

# find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
# find_library(AVFORMAT_LIBRARY avformat)

# find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
# find_library(AVUTIL_LIBRARY avutil)

# find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
# find_library(AVDEVICE_LIBRARY avdevice)

include(${CMAKE_SOURCE_DIR}/Findudev.cmake)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(OpenCV CONFIG REQUIRED PATHS /home/mers/Desktop/Github/opencv_dir/build)

message(STATUS "${CMAKE_LIBRARY_PATH}")

add_executable(stream src/programatic_stream.cc)
target_link_libraries(stream CameraSDK udev)

add_executable(signalslot src/playground/signal_slot.cpp)
target_link_libraries(signalslot PRIVATE CameraSDK ${Boost_LIBRARY})

# add_executable(equirect src/playground/equirectangular_convert.cpp)
# target_include_directories(equirect PUBLIC ${OpenCV_INCLUDE_DIRS})
# target_link_libraries(equirect PRIVATE CameraSDK ${Boost_LIBRARY} ${OpenCV_LIBS})

add_executable(camera_test src/examples/insta360/cameraSDKTest.cc)
target_link_libraries(camera_test CameraSDK udev)

add_executable(connect src/examples/insta360/connect.cc)
target_link_libraries(connect CameraSDK udev)

add_executable(recording src/examples/insta360/recording.cc)
target_link_libraries(recording CameraSDK udev)

add_executable(settings src/examples/insta360/settings.cc)
target_link_libraries(settings CameraSDK udev)

add_executable(camcontrol src/examples/camcontrol/CameraController.cpp)
target_include_directories(camcontrol PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(camcontrol CameraSDK udev ${Boost_LIBRARY} ${OpenCV_LIBS} ${AVCODEC_LIBRARY} ${AVUTIL_LIBRARY})
# target_include_directories(camcontrol PRIVATE ${OpenCV_INCLUDE_DIRS} ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR})
# target_link_libraries(camcontrol PRIVATE CameraSDK udev ${Boost_LIBRARY} ${OpenCV_LIBS} ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY})

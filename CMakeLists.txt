cmake_minimum_required(VERSION 3.12)
project(Cam)

#------------------------------基本设置------------------------------
set(CMAKE_CXX_STANDARD 20)
set(MVCAM_COMMON_PATH /opt/MVS)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#------------------------------包含头文件------------------------------
include_directories(
    ${MVCAM_COMMON_PATH}/include
)

#------------------------------查找库文件------------------------------
find_package(OpenCV REQUIRED)
find_package(realsense2 REQUIRED)


add_executable(test main.cpp hikcam.hpp cameras.hpp)

target_link_libraries(test
    MvCameraControl 
    ${OpenCV_LIBS} 
    ${realsense2_LIBRARY}
)

cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME opencv_demos)
message(STATUS "PROJECT_NAME: " ${PROJECT_NAME})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
project(${PROJECT_NAME})


# 设置OpenCV目录
set(OpenCV_DIR D:/01-study/02-opensource/opencv-4.8.0/build/install)
# 搜索OpenCV目录
find_package(OpenCV REQUIRED)

# add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
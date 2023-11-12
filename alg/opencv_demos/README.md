## 一、win10使用cmake编译opencv
### 1. 下载opencv和opencv_contrib源码
- OpenCV仓库地址：https://github.com/opencv/opencv.git  
- OpenCV-contrib仓库地址：https://github.com/opencv/opencv_contrib.git
### 2. 生成Makefile
使用CMake的GUI界面，参考https://blog.songjiahao.com/archives/352  
生成Makefile
### 3. 编译和安装
- 编译：`mingw32-make -j8`
- 安装：`mingw32-make install`

### 参考链接
[1] [win10平台使用CMake编译opencv](https://blog.songjiahao.com/archives/352)

二、CMakeLists.txt导入OpenCV
1. 加入环境变量：
`D:\01-study\02-opensource\opencv-4.8.0\build\install\x64\mingw\bin`
2. 编写CMakeLists.txt
```
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
```
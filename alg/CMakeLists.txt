cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME "Alg")
project(${PROJECT_NAME})

option(PRINT_CMAKE_LOG "print cmake log" ON)
if (PRINT_CMAKE_LOG)
    message("--------------------------------")
    message(STATUS ${PROJECT_NAME}, " BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
    message("[CMAKE INFO] PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR}) # 打印信息
endif (PRINT_CMAKE_LOG)

set(OpenCV_DIR "D:/02-software/opencv-4.5.2/build_install")
find_package(OpenCV 4 REQUIRED)

# 把当前目录下的所有源文件添加到SRC_LIST中 
aux_source_directory(. SRC_LIST)

# 添加头文件
# include_directories(${OpenCV_DIR} include/)

# 添加Demo-Yolo子目录
add_subdirectory(Demo-YOLO)

# 添加一个可执行程序
add_executable(${PROJECT_NAME} main.cpp)

# 添加链接库
target_link_libraries(${PROJECT_NAME} 
                      PRIVATE
                      ${OpenCV_LIBS}
                      YOLO_LIB)
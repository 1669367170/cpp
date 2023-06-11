cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME "SortTrackerDemo")
project(${PROJECT_NAME})

set(OpenCV_DIR "D:/02-software/opencv-4.5.2/build_install")
find_package(OpenCV 4 REQUIRED)

# 把当前目录下的所有源文件添加到SRC_LIST中 
aux_source_directory(. SRC_LIST)
# list追加变量的值
list(APPEND SRC_LIST src/main.cpp)

message("[CMAKE INFO] PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR}) # 打印信息
# 添加头文件
include_directories(${OpenCV_DIR} ${PROJECT_SOURCE_DIR}/include)

# 添加一个可执行程序
add_executable(${PROJECT_NAME} ${SRC_LIST})

# target_include_directories(${PROJECT_NAME}
#                             PBULIC
#                             ${OpenCV_DIR})
target_link_libraries(${PROJECT_NAME} 
                      PRIVATE
                      ${OpenCV_LIBS})
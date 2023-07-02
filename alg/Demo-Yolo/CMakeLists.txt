set(PROJECT_NAME YOLO_LIB)
message(STATUS "STATIC LIB: " ${PROJECT_NAME})

set(OpenCV_DIR "D:/02-software/opencv-4.5.2/build_install")
find_package(OpenCV 4 REQUIRED)

# 递归搜索
file(GLOB_RECURSE H_SRCS "*.h")
file(GLOB_RECURSE CXX_SRCS "*.cpp")

list(APPEND SRCS ${H_SRCS} ${CXX_SRCS})

# 生成（静态）链接库
add_library(${PROJECT_NAME} STATIC ${SRCS})

target_link_libraries(${PROJECT_NAME} 
                      PRIVATE
                      ${OpenCV_LIBS})
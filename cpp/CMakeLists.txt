# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(RealTimeFilter)

# 指定 OpenCV 路径（Homebrew 安装路径）
set(OpenCV_DIR "/opt/homebrew/opt/opencv@4/share/OpenCV")
set(CMAKE_CXX_STANDARD 11)

# 查找 OpenCV 库
find_package(OpenCV REQUIRED)

# 查找线程库
find_package(Threads REQUIRED)

# 包含头文件
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
)

# 生成可执行文件
add_executable(main
    src/main.cpp
    src/filters/edge_detection.cpp
    src/filters/gaussian_blur.cpp
    src/filters/hdr_tone_mapping.cpp
    src/filters/vintage.cpp
    src/filters/cartoon.cpp
    src/utils/on_screen_display.cpp
)

# 链接 OpenCV 库
target_link_libraries(main ${OpenCV_LIBS})
# 链接 线程库
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
cmake_minimum_required(VERSION 3.10)
project(Project1)

# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加可执行文件（common 为头文件，故不用引用不存在的 .cpp）
add_executable(main_A cmd/A/main.cpp)
add_executable(main_B cmd/B/main.cpp)
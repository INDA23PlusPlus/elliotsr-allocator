cmake_minimum_required(VERSION 3.0.0)
project(allocator VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

add_executable(allocator main.c buddy.c)
target_link_libraries(allocator m)
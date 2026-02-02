cmake_minimum_required(VERSION 3.15)
project(shared_ptr_lesson LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shared_ptr_lesson
    main.cpp
    console_color.cpp
    texture.cpp
    shape.cpp
    canvas.cpp
    textures_factory.cpp
    dashed_border.cpp
    utf8_table.cpp
    lesson_print.cpp
)

if (WIN32)
  target_compile_definitions(shared_ptr_lesson PRIVATE _WIN32)
endif()

target_compile_options(shared_ptr_lesson PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-O2 -Wall -Wextra -Wpedantic>
)

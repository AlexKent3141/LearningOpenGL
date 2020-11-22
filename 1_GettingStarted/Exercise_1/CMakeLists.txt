cmake_minimum_required (VERSION 3.16.3)
project(test)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
  "./include"
  ${GLFW3_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIRS})

add_executable(test
  main.cpp
  glad.c)

target_link_libraries(test
  dl
  glfw
  OpenGL::GL)
    

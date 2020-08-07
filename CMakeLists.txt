cmake_minimum_required(VERSION 3.16)
project(repo)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -Wno-unused-result -Wno-deprecated-declarations")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

add_executable(8.6 main.cpp global_values.cpp mask_simulation.h input_functions.cpp openGL_basic_functions.cpp openGL_structure_functions.cpp sir_functions.cpp time_initialization.cpp)
target_link_libraries(8.6 ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)
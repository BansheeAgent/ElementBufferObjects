cmake_minimum_required(VERSION 3.16)
project(ElementBufferObjects)

set(CMAKE_CXX_STANDARD 14)

add_executable(ElementBufferObjects main.cpp lib/glad/src/glad.c)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/glm)

target_link_libraries(ElementBufferObjects glfw ${GLFW_LIBRARY})
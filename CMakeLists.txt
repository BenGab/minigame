cmake_minimum_required(VERSION 3.20)
project(minigame)

file(GLOB MINI_SRC src/*.cpp)
add_executable(minigame ${MINI_SRC})
target_include_directories(minigame PRIVATE include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(minigame PRIVATE glm::glm ${OPENGL_LIBRARY} glfw ${GLEW_LIBRARIES})
include_directories("${GLEW_INCLUDE_DIR}")


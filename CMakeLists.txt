cmake_minimum_required(VERSION 3.16)
project(openglProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/include/extern"
        "${CMAKE_SOURCE_DIR}/include/intern"
)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include/extern/GLFW/include)
link_directories(${CMAKE_SOURCE_DIR}/include/extern/GLFW/libs)

include_directories(${CMAKE_SOURCE_DIR}/include/extern/GLEW/include/GL)
link_directories(${CMAKE_SOURCE_DIR}/include/extern/GLEW/libs)

include_directories(${CMAKE_SOURCE_DIR}/include/extern/assimp/include)
link_directories(${CMAKE_SOURCE_DIR}/include/extern/assimp/libs)

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/extern/imgui/*.c
        ${CMAKE_SOURCE_DIR}/include/extern/imgui/*.cpp
        ${CMAKE_SOURCE_DIR}/include/extern/stb_image/*.cpp
        )


file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp
        ${CMAKE_SOURCE_DIR}/include/intern/*.h
        ${CMAKE_SOURCE_DIR}/include/extern/imgui/*.h
        ${CMAKE_SOURCE_DIR}/include/extern/imgui/*.hpp
        ${CMAKE_SOURCE_DIR}/include/extern/stb_image/*.h
        )


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} include/intern/imfilebrowser.h)

target_link_libraries(openglProject libglfw.so.3.3 libGLEW.a libassimp.so.5.0.0 ${OPENGL_LIBRARIES})
cmake_minimum_required(VERSION 3.17)
project(Blatt11_Aufgabe29)

# Set the C++ version
set(CMAKE_CXX_STANDARD 17)

# Set the build directory
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/)

# Set the path for cmake to look for modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
if (WIN32)
    # Include it only, when compiling on windows
    set(GLFW3_ROOT "${PROJECT_SOURCE_DIR}/GLFW")
elseif(APPLE)
    message(FATAL_ERROR "Compilation not setup for Apple systems.")
endif ()
find_package(GLFW3 REQUIRED)

# GLAD
set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/GLAD/include")
set(GLAD_SOURCES ${PROJECT_SOURCE_DIR}/GLAD/src/glad.c)
add_library(glad ${GLAD_SOURCES})

# ImGUI
set(IMGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/ImGUI")
set(IMGUI ImGUI/imconfig.h
        ImGUI/imgui.cpp
        ImGUI/imgui.h
        ImGUI/imgui_draw.cpp
        ImGUI/imgui_impl_glfw.cpp
        ImGUI/imgui_impl_glfw.h
        ImGUI/imgui_impl_opengl3.cpp
        ImGUI/imgui_impl_opengl3.h
        ImGUI/imgui_internal.h
        ImGUI/imgui_widgets.cpp
        ImGUI/imstb_rectpack.h
        ImGUI/imstb_textedit.h
        ImGUI/imstb_truetype.h)

# Include required directories
include_directories(${GLFW3_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR})

# Define sources
set(SOURCE src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Scene.h
        src/scenes/KochCurveScene.cpp
        src/scenes/KochCurveScene.h)

add_executable(Blatt11_Aufgabe29 ${SOURCE} ${IMGUI})

# Link libraries
target_link_libraries(Blatt11_Aufgabe29 ${OPENGL_LIBRARIES} glad ${GLFW3_LIBRARY} ${CMAKE_DL_LIBS})
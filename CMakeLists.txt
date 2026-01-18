cmake_minimum_required(VERSION 3.16)
project(n-body_simulator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/misc/cpp/imgui_stdlib.cpp
)

# Include headers
include_directories(include)

# Executable
add_executable(n-body_simulator
    src/Body.cpp
    src/Universe.cpp
    src/Gravity.cpp
    src/gui/Gui.cpp
    ${IMGUI_SOURCES}
)

# Include paths
target_include_directories(n-body_simulator PRIVATE
    external/imgui
    external/imgui/backends
    external/imgui/misc/cpp/
)

# Link libraries
target_link_libraries(n-body_simulator PRIVATE
    OpenGL::GL
    glfw
    glad::glad
    dl
)

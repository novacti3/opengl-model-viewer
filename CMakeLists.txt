cmake_minimum_required(VERSION 3.15)

if(APPLE)
    message(FATAL "macOS is not supported. Abandoning build...")
endif()

project(ModelViewer LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/")

file(COPY res/internal DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")
file(COPY res/internal DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/res")
file(COPY res/internal DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/res")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link GLFW and set build options
add_subdirectory(libs/glfw ${ModelViewer_BINARY_DIR}/glfw)
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

list(APPEND INCLUDES
    # external includes
    libs/
    libs/glfw/include
    libs/glad/include
    libs/glm/
    libs/imgui/
    libs/tinyobjloader/

    # project includes
    src/
)

list(APPEND SOURCES
    # external sources
    libs/glad/src/glad.c
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp

    # project core sources
    src/core/resource_manager.cpp
    src/core/ui_manager.cpp

    # project rendering sources
    src/rendering/renderer.cpp
    src/rendering/shader_uniform.cpp
    src/rendering/shader.cpp
    src/rendering/texture.cpp
    src/rendering/model.cpp
)

add_executable(ModelViewer src/program.cpp)
# Change the executable's name and C++ language standard
set_target_properties(ModelViewer PROPERTIES 
OUTPUT_NAME ModelViewer 
CXX_STANDARD 17)

target_link_libraries(ModelViewer OpenGL::GL glfw)

target_include_directories(ModelViewer PRIVATE ${INCLUDES})
target_sources(ModelViewer PRIVATE ${SOURCES})
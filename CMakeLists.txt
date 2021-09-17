cmake_minimum_required(VERSION 3.15)

if(APPLE)
    message(FATAL "macOS is not supported. Abandoning build...")
endif()

project(ModelViewer LANGUAGES C CXX)

make_directory(bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/$<CONFIG>/)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link GLFW and set build options
 add_subdirectory(libs/glfw ${ModelViewer_BINARY_DIR}/glfw)
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(libs/freetype ${ModelViewer_BINARY_DIR}/freetype)

list(APPEND INCLUDES
    # external includes
    libs/glfw/include
    libs/glad/include
    libs/glm/
    libs/freetype/include

    # project includes
    src/
)

list(APPEND SOURCES
    # external sources
    libs/glad/src/glad.c
)

add_executable(ModelViewer src/program.cpp)
# Change the executable's name and C++ language standard
set_target_properties(ModelViewer PROPERTIES 
OUTPUT_NAME ModelViewer 
CXX_STANDARD 17)

target_link_libraries(ModelViewer OpenGL::GL glfw freetype)

target_include_directories(ModelViewer PRIVATE ${INCLUDES})
target_sources(ModelViewer PRIVATE ${SOURCES})
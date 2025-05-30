cmake_minimum_required(VERSION 3.13)
include(CMakePrintHelpers)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_BUILD_TYPE Debug)
project(ProjectJay)

file(GLOB SOURCES
    Source/**/*.cpp
)

set(BGFX_AMALGAMATE ON CACHE BOOL "")
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "")
set(BGFX_CONFIG_DEBUG OFF CACHE BOOL "")
set(BGFX_CUSTOM_TARGETS OFF CACHE BOOL "")
set(BGFX_INSTALL OFF CACHE BOOL "")
set(BGFX_INSTALL_EXAMPLES OFF CACHE BOOL "")
set(BGFX_USE_DEBUG_SUFFIX OFF CACHE BOOL "")
set(BGFX_USE_OVR OFF CACHE BOOL "")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(BX_AMALGAMATED ON  CACHE BOOL "")
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/bgfx.cmake)
include_directories("Source/")
include_directories("Libraries/bgfx.cmake/include")
target_compile_definitions(bgfx PRIVATE "WL_EGL_PLATFORM=1")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
set(GLFW_USE_OSMESA OFF CACHE BOOL "")
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "")
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "")
set(GLFW_BUILD_X11 OFF CACHE BOOL "")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/glfw)
include_directories("Libraries/glfw/include")
set_target_properties(update_mappings PROPERTIES FOLDER "GLFW3")

include_directories("Libraries/FastNoise/Cpp")
include_directories("Libraries/glm")
include_directories("Libraries/json/include")
include_directories("Libraries/vector_const")
add_compile_definitions(_DEBUG)
add_compile_definitions(_PC)
add_executable(ProjectJay ${SOURCES})

find_package(OpenMP REQUIRED)
if (LINUX)
    find_library(WAYLAND_CLIENT wayland-client)
    find_library(WAYLAND_EGL wayland-egl)
else()
    set(WAYLAND_CLIENT "")
    set(WAYLAND_EGL "")
endif()
cmake_print_variables(WAYLAND_CLIENT)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw bgfx "${WAYLAND_CLIENT}" "${WAYLAND_EGL}" OpenMP::OpenMP_CXX)

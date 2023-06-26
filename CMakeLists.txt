cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Output)
project(ProjectJay)

file(GLOB SOURCES
    Source/Game/Entity/Entity.cpp
    Source/Game/Entity/EntityManager.cpp

    Source/Game/Systems/CollisionSystem.cpp
    Source/Game/Systems/GroundTraceSystem.cpp
    Source/Game/Systems/MovementSystem.cpp
    Source/Game/Systems/TransformSystem.cpp
    Source/Game/Systems/SpreadActivatorSystem.cpp
    Source/Game/Systems/SpreadDetectSystem.cpp
    Source/Game/Systems/GroundStickSystem.cpp
    Source/Game/Systems/VelocitySystem.cpp
    Source/Game/Systems/ProjectileSystem.cpp
    Source/Game/Systems/IntersectSystem.cpp
    Source/Game/Systems/IntervalSpawnSystem.cpp

    Source/Helpers/ChunkHelpers.cpp
    Source/Helpers/LoadHelpers.cpp

    Source/Game/Camera.cpp
    Source/Game/Game.cpp
    Source/Game/PlayerController.cpp
    Source/Game/Time.cpp

    Source/Game/World/World.cpp
    Source/Game/World/SeedManager.cpp
    Source/Game/World/SpreadManager.cpp

    Source/Logging/Logger.cpp
    Source/Logging/ScreenText.cpp

    Source/Rendering/Renderer.cpp

    Source/Resource/AnimationResource.cpp
    Source/Resource/ResourceManager.cpp
    Source/Resource/SkeletonResource.cpp

    Source/Types/Gamepad.cpp
    Source/Types/Transform.cpp

    IF(WIN32)
        Source/Game/World/Win_World.cpp
        Source/Game/World/Win_SpreadManager.cpp
        Source/Logging/Win_Logger.cpp
        Source/Resource/Win_ResourceManager.cpp
        Source/Resource/Win_SkeletonResource.cpp

    Source/Rendering/PC_Renderer.cpp
    Source/Game/PC_Game.cpp
    Source/Platform/PC_Platform.cpp
    Source/Entry/PC_Main.cpp
)

set(BGFX_AMALGAMATED        OFF CACHE INTERNAL "" FORCE)
set(BGFX_BUILD_EXAMPLES     OFF CACHE INTERNAL "" FORCE)
set(BGFX_CONFIG_DEBUG       OFF CACHE INTERNAL "" FORCE)
set(BGFX_CUSTOM_TARGETS     OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL            OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL_EXAMPLES   OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_DEBUG_SUFFIX   OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_OVR            OFF CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS       OFF CACHE INTERNAL "" FORCE)
set(BX_AMALGAMATED          OFF CACHE INTERNAL "" FORCE)
set(BGFX_BUILD_TOOLS        OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/bgfx.cmake)
include_directories("Libraries/bgfx.cmake/include")

set(GLFW_BUILD_DOCS         OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_EXAMPLES     OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_TESTS        OFF CACHE INTERNAL "" FORCE)
set(GLFW_INSTALL            OFF CACHE INTERNAL "" FORCE)
set(GLFW_USE_OSMESA         OFF CACHE INTERNAL "" FORCE)
set(GLFW_VULKAN_STATIC      OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/glfw)
include_directories("Libraries/glfw/include")

include_directories("Libraries/FastNoise")
include_directories("Libraries/glm")
include_directories("Libraries/json")
add_compile_definitions(_DEBUG)
add_compile_definitions(_PC)
add_executable(ProjectJay ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw bgfx)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output)
set(CMAKE_BUILD_TYPE Debug)
project(ProjectJay)

file(GLOB SOURCES
    Source/Game/Entity/Entity.cpp
    Source/Game/Entity/EntityManager.cpp

    Source/Game/Systems/PushSystem.cpp
    Source/Game/Systems/DestroyMeterSystem.cpp
    Source/Game/Systems/FreezeSystem.cpp
    Source/Game/Systems/GroundTraceSystem.cpp
    Source/Game/Systems/MovementSystem.cpp
    Source/Game/Systems/TransformSystem.cpp
    Source/Game/Systems/SpreadActivatorSystem.cpp
    Source/Game/Systems/SpreadDetectSystem.cpp
    Source/Game/Systems/TrampleSystem.cpp
    Source/Game/Systems/GroundStickSystem.cpp
    Source/Game/Systems/VelocitySystem.cpp
    Source/Game/Systems/ProjectileSystem.cpp
    Source/Game/Systems/IntervalSpawnSystem.cpp
    Source/Game/Systems/SkeletonSystem.cpp

    Source/Helpers/LoadHelpers.cpp
    Source/Helpers/Random.cpp

    Source/Game/AI/WalkerController.cpp

    Source/Game/Camera.cpp
    Source/Game/Game.cpp
    Source/Game/PlayerController.cpp
    Source/Game/Time.cpp
    Source/Game/ParticleEmitter.cpp

    Source/Game/World/World.cpp
    Source/Game/ParticleManager.cpp
    Source/Game/World/SeedManager.cpp
    Source/Game/World/SpreadManager.cpp

    Source/Logging/Logger.cpp
    Source/Logging/ScreenText.cpp

    Source/Rendering/Renderer.cpp
    Source/Rendering/Skeleton.cpp

    Source/Types/Gamepad.cpp
    Source/Types/Transform.cpp

    Source/Rendering/PC_Renderer.cpp
    Source/Game/PC_Game.cpp
    Source/Platform/PC_Platform.cpp
    Source/Entry/PC_Main.cpp
    Source/Logging/PC_ScreenText.cpp
)

set(BGFX_AMALGAMATED            ON  CACHE INTERNAL "" FORCE)
set(BGFX_BUILD_EXAMPLES         OFF CACHE INTERNAL "" FORCE)
set(BGFX_CONFIG_DEBUG           OFF CACHE INTERNAL "" FORCE)
set(BGFX_CUSTOM_TARGETS         OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL                OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL_EXAMPLES       OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_DEBUG_SUFFIX       OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_OVR                OFF CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS           OFF CACHE INTERNAL "" FORCE)
set(BX_AMALGAMATED              ON  CACHE INTERNAL "" FORCE)
set(BGFX_BUILD_TOOLS            OFF CACHE INTERNAL "" FORCE)

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
set_target_properties(update_mappings PROPERTIES FOLDER "GLFW3")

include_directories("Libraries/FastNoise/Cpp")
include_directories("Libraries/glm")
include_directories("Libraries/json/include")
include_directories("Libraries/vector_const")
add_compile_definitions(_DEBUG)
add_compile_definitions(_PC)
add_executable(ProjectJay ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw bgfx)

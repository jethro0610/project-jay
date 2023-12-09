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
    Source/Entity/Entity.cpp
    Source/Entity/EntityManager.cpp

    Source/Systems/PushSystem.cpp
    Source/Systems/HitSystem.cpp
    Source/Systems/DestroyMeterSystem.cpp
    Source/Systems/FreezeSystem.cpp
    Source/Systems/GroundTraceSystem.cpp
    Source/Systems/MovementSystem.cpp
    Source/Systems/TransformSystem.cpp
    Source/Systems/SpreadActivatorSystem.cpp
    Source/Systems/SpreadDetectSystem.cpp
    Source/Systems/TrampleSystem.cpp
    Source/Systems/GroundStickSystem.cpp
    Source/Systems/VelocitySystem.cpp
    Source/Systems/ProjectileSystem.cpp
    Source/Systems/IntervalSpawnSystem.cpp
    Source/Systems/SkeletonSystem.cpp
    Source/Systems/SpawnSystem.cpp
    Source/Systems/SeedOnDestroySystem.cpp
    Source/Systems/DestroyOnBoundSystem.cpp
    Source/Systems/ParticleAttachSystem.cpp

    Source/Helpers/LoadHelpers.cpp
    Source/Helpers/Random.cpp

    Source/Collision/Collision.cpp

    Source/AI/WalkerController.cpp

    Source/Camera/Camera.cpp

    Source/Controller/PlayerController.cpp

    Source/Time/Time.cpp
    
    Source/Particle/ParticleEmitter.cpp
    Source/Particle/ParticleManager.cpp

    Source/Level/LevelLoader.cpp

    Source/Resource/ResourceManager.cpp
    Source/Resource/PC_ResourceManager.cpp

    Source/Terrain/Terrain.cpp

    Source/Seed/SeedManager.cpp

    Source/Spread/SpreadManager.cpp

    Source/Logging/Logger.cpp
    Source/Logging/ScreenText.cpp
    Source/Logging/PC_ScreenText.cpp

    Source/Rendering/Renderer.cpp
    Source/Rendering/Skeleton.cpp
    Source/Rendering/PC_Renderer.cpp

    Source/Types/Gamepad.cpp
    Source/Types/Noise.cpp
    Source/Types/Transform.cpp

    Source/Platform/PC_Platform.cpp

    Source/Game/Game.cpp
    Source/Game/PC_Game.cpp
    Source/Entry/PC_Main.cpp
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
include_directories("Source/")
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

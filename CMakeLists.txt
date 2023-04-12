cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ProjectJay)

file(GLOB SOURCES
    Libraries/WICTextureLoader/WICTextureLoader11.cpp
	Source/Game/Entity/Entity.cpp
	Source/Game/Entity/EntityManager.cpp

	Source/Game/Systems/CollisionSystem.cpp
	Source/Game/Systems/GroundTraceSystem.cpp
	Source/Game/Systems/MovementSystem.cpp
	Source/Game/Systems/TransformSystem.cpp
    Source/Game/Systems/TerrainModSystem.cpp
    Source/Game/Systems/SpreadActivatorSystem.cpp
    Source/Game/Systems/SpreadDetectSystem.cpp
    Source/Game/Systems/PickupSystem.cpp
    Source/Game/Systems/GroundStickSystem.cpp
    Source/Game/Systems/VelocitySystem.cpp
    Source/Game/Systems/ProjectileSystem.cpp
    Source/Game/Systems/IntersectSystem.cpp
    Source/Game/Systems/IntervalSpawnSystem.cpp

    Source/Game/LoadHelpers.cpp
	Source/Game/Camera.cpp
	Source/Game/Game.cpp
	Source/Game/World.cpp
    Source/Game/SpreadManager.cpp
    Source/Game/PlayerController.cpp

	Source/Logging/Logger.cpp
    Source/Logging/ScreenText.cpp

	Source/Rendering/Renderer.cpp

	Source/Resource/AnimationResource.cpp
	Source/Resource/RawModel.cpp
	Source/Resource/ResourceManager.cpp
	Source/Resource/SkeletonResource.cpp

	Source/Types/Gamepad.cpp
	Source/Types/Transform.cpp

    IF(WIN32)
		Source/Game/Win_Game.cpp
		Source/Game/Win_World.cpp
        Source/Game/Win_SpreadManager.cpp
		Source/Logging/Win_Logger.cpp
		Source/Rendering/Win_Renderer.cpp
		Source/Resource/Win_RawModel.cpp
		Source/Resource/Win_ResourceManager.cpp
		Source/Resource/Win_SkeletonResource.cpp
		Source/Windows/Win_Main.cpp
		Source/Windows/WindowsLayer.cpp
		Source/Resource/DXResources.cpp
)

include_directories("Libraries/glm")
include_directories("Libraries/json")
include_directories("Libraries/WICTextureLoader")
include_directories("Libraries/FastNoise")
add_compile_definitions(_DEBUG)
if(MINGW)
    add_compile_definitions(_WINDOWS)
endif()
add_executable(ProjectJay WIN32 ${SOURCES}) # Headers are added into the VS solution

if (MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib xinput.lib d3dcompiler.lib --static)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib xinput.lib d3dcompiler.lib)
endif()

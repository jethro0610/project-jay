cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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

    Source/Game/World/World.cpp
    Source/Game/World/SpreadManager.cpp

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
        Source/Game/World/Win_World.cpp
        Source/Game/World/Win_SpreadManager.cpp
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
include_directories("Libraries/FastNoise")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/DirectXTK ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK)
add_compile_definitions(_DEBUG)
add_compile_definitions(NOMINMAX)
add_executable(ProjectJay WIN32 ${SOURCES}) # Headers are added into the VS solution
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK d3d11.lib Xinput.lib d3dcompiler.lib)

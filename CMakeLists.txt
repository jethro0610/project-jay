cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ProjectJay)

# Headers can be added to the executable so they show up in the VS solution
file(GLOB HEADERS
	Source/Game/Components/ColliderComponent.h
	Source/Game/Components/Components.h
	Source/Game/Components/GroundTraceComponent.h
	Source/Game/Components/MovementComponent.h
	Source/Game/Components/StaticModelComponent.h
	Source/Game/Components/TransformComponent.h
	Source/Game/Components/VelocityComponent.h
    Source/Game/Components/SpreadActivatorComponent.h
    Source/Game/Components/SpreadDetectComponent.h
    Source/Game/Components/PickupComponent.h
    Source/Game/Components/HoldableComponent.h

	Source/Game/Entity/Entity.h
	Source/Game/Entity/EntityManager.h

	Source/Game/Systems/CollisionSystem.h
	Source/Game/Systems/GroundTraceSystem.h
	Source/Game/Systems/MovementSystem.h
	Source/Game/Systems/PlayerInputSystem.h
	Source/Game/Systems/TransformSystem.h
	Source/Game/Systems/Systems.h
    Source/Game/Systems/TerrainModSystem.h
    Source/Game/Systems/SpreadActivatorSystem.h
    Source/Game/Systems/SpreadDetectSystem.h

	Source/Game/Camera.h
	Source/Game/Game.h
	Source/Game/TimeConstants.h
	Source/Game/World.h
	Source/Game/WorldConstants.h
    Source/Game/SpreadManager.h

	Source/Logging/Logger.h

	Source/Rendering/Renderer.h

	Source/Resource/AnimationResource.h
	Source/Resource/RawModel.h
	Source/Resource/ResourceManager.h
	Source/Resource/SkeletonResource.h

	Source/Types/Gamepad.h
	Source/Types/Transform.h

	IF(WIN32)
		Source/Windows/WindowsLayer.h
		Source/Resource/DXResources.h
)

file(GLOB SOURCES
	Source/Game/Components/Components.cpp

	Source/Game/Entity/Entity.cpp
	Source/Game/Entity/EntityManager.cpp

	Source/Game/Systems/CollisionSystem.cpp
	Source/Game/Systems/GroundTraceSystem.cpp
	Source/Game/Systems/MovementSystem.cpp
	Source/Game/Systems/PlayerInputSystem.cpp
	Source/Game/Systems/TransformSystem.cpp
    Source/Game/Systems/TerrainModSystem.cpp
    Source/Game/Systems/SpreadActivatorSystem.cpp
    Source/Game/Systems/SpreadDetectSystem.cpp
    Source/Game/Systems/PickupSystem.cpp
    Source/Game/Systems/GroundStickSystem.cpp
    Source/Game/Systems/VelocitySystem.cpp
    Source/Game/Systems/ProjectileSystem.cpp
    Source/Game/Systems/HitboxSystem.cpp

	Source/Game/Camera.cpp
	Source/Game/Game.cpp
	Source/Game/World.cpp
    Source/Game/SpreadManager.cpp

	Source/Logging/Logger.cpp

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
include_directories("Libraries/FastNoise")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/DirectXTK ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK)
add_compile_definitions(_DEBUG)
add_executable(ProjectJay WIN32 ${SOURCES} ${HEADERS}) # Headers are added into the VS solution
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK d3d11.lib Xinput.lib d3dcompiler.lib)

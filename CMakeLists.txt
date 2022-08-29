cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ProjectJay)

file(GLOB SOURCES
	Source/Game/Components/Components.cpp

	Source/Game/Entity/Entity.cpp
	Source/Game/Entity/EntityManager.cpp

	Source/Game/Systems/CollisionSystem.cpp
	Source/Game/Systems/GroundTraceSystem.cpp
	Source/Game/Systems/MovementSystem.cpp
	Source/Game/Systems/PlayerInputSystem.cpp
	Source/Game/Systems/TransformSystem.cpp

	Source/Game/Camera.cpp
	Source/Game/Game.cpp
	Source/Game/Win_Game.cpp
	Source/Game/World.cpp
	Source/Game/Win_World.cpp

	Source/Logging/Logger.cpp
	Source/Logging/Win_Logger.cpp

	Source/Rendering/Renderer.cpp
	Source/Rendering/Win_Renderer.cpp

	Source/Resource/AnimationResource.cpp
	Source/Resource/Win_AnimationResource.cpp
	Source/Resource/DXResources.cpp
	Source/Resource/RawModel.cpp
	Source/Resource/Win_RawModel.cpp
	Source/Resource/ResourceManager.cpp
	Source/Resource/Win_ResourceManager.cpp
	Source/Resource/SkeletonResource.cpp
	Source/Resource/Win_SkeletonResource.cpp

	Source/Types/Gamepad.cpp
	Source/Types/Transform.cpp

	Source/Windows/Win_Main.cpp
	Source/Windows/WindowsLayer.cpp
)

include_directories("Libraries/glm")
include_directories("Libraries/FastNoise")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/DirectXTK ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK)
add_compile_definitions(WINDOWS)
add_compile_definitions(_DEBUG)
add_executable(ProjectJay WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK d3d11.lib Xinput.lib d3dcompiler.lib)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ProjectJay)

file(GLOB SOURCES
	project-jay/Game/Components/Components.cpp

	project-jay/Game/Entity/Entity.cpp
	project-jay/Game/Entity/EntityManager.cpp

	project-jay/Game/Systems/CollisionSystem.cpp
	project-jay/Game/Systems/GroundTraceSystem.cpp
	project-jay/Game/Systems/MovementSystem.cpp
	project-jay/Game/Systems/PlayerInputSystem.cpp
	project-jay/Game/Systems/TransformSystem.cpp

	project-jay/Game/Camera.cpp
	project-jay/Game/Game.cpp
	project-jay/Game/Win_Game.cpp
	project-jay/Game/World.cpp
	project-jay/Game/Win_World.cpp

	project-jay/Logging/Logger.cpp
	project-jay/Logging/Win_Logger.cpp

	project-jay/Rendering/Renderer.cpp
	project-jay/Rendering/Win_Renderer.cpp

	project-jay/Resource/AnimationResource.cpp
	project-jay/Resource/Win_AnimationResource.cpp
	project-jay/Resource/DXResources.cpp
	project-jay/Resource/RawModel.cpp
	project-jay/Resource/Win_RawModel.cpp
	project-jay/Resource/ResourceManager.cpp
	project-jay/Resource/Win_ResourceManager.cpp
	project-jay/Resource/SkeletonResource.cpp
	project-jay/Resource/Win_SkeletonResource.cpp

	project-jay/Types/Gamepad.cpp
	project-jay/Types/Transform.cpp

	project-jay/Windows/Win_Main.cpp
	project-jay/Windows/WindowsLayer.cpp
)

include_directories("Libraries/glm")
include_directories("Libraries/FastNoise")
add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/DirectXTK ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK)
add_compile_definitions(WINDOWS)
add_compile_definitions(_DEBUG)
add_executable(ProjectJay WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK d3d11.lib Xinput.lib d3dcompiler.lib)

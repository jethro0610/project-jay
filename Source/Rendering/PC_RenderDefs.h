#pragma once
#include <bgfx/bgfx.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#ifdef __linux__ 
    #define GLFW_EXPOSE_NATIVE_X11
    #define GETHANDLE(window) (void*)(uintptr_t)glfwGetX11Window(window)
#elif _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GETHANDLE(window) glfwGetWin32Window(window)
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef bgfx::UniformHandle Uniform;
typedef bgfx::ShaderHandle Shader;
typedef bgfx::VertexBufferHandle VertexBuffer;
typedef bgfx::IndexBufferHandle IndexBuffer;
typedef bgfx::ProgramHandle MaterialShader;

typedef bgfx::TextureHandle Texture;
typedef bgfx::UniformHandle TextureSampler;

typedef bgfx::FrameBufferHandle FrameBuffer;

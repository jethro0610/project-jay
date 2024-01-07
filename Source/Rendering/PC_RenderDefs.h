#pragma once
#include <bgfx/bgfx.h>

typedef bgfx::UniformHandle UniformHandle;
typedef bgfx::ShaderHandle VertexShaderHandle;
typedef bgfx::ShaderHandle FragmentShaderHandle;
typedef bgfx::VertexBufferHandle VertexBufferHandle;
typedef bgfx::IndexBufferHandle IndexBufferHandle;
typedef bgfx::InstanceDataBuffer InstanceBufferHandle;
typedef bgfx::ProgramHandle ShaderHandle;

typedef bgfx::TextureHandle TextureHandle;
typedef bgfx::UniformHandle TextureSamplerHandle;

typedef bgfx::FrameBufferHandle FrameBufferHandle;

typedef int View;

#define DEFAULT_HANDLE BGFX_INVALID_HANDLE

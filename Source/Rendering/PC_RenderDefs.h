#include <bgfx/bgfx.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#if BX_PLATFORM_LINUX
    #define GLFW_EXPOSE_NATIVE_X11
    #define GETHANDLE(window) glfwGetX11Window(window)
#else
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GETHANDLE(window) glfwGetWin32Window(window)
#endif

typedef bgfx::UniformHandle Uniform;
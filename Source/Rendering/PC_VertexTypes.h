#include <bgfx/bgfx.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct StaticVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 uv;

    static bgfx::VertexLayout layout;
    static void Init() {
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

struct SkeletalVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 uv;
    glm::u8vec4 bones;
    glm::vec4 weights;

    static bgfx::VertexLayout layout;
    static void Init() {
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Float)
            .end();
    }
};

struct WorldVertex {
    glm::vec3 position;
    static bgfx::VertexLayout layout;
    static void Init() {
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();
    }
};

struct TextureQuadVertex {
    glm::vec2 position;
    glm::vec2 uv;
    static bgfx::VertexLayout layout;
    static void Init() {
        layout
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

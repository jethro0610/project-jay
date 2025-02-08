#pragma once
#include "RenderDefs.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Model.h"
#include "Material.h"
#include "Entity/EntityList.h"
#include "Text/Text.h"
#include "Spread/SpreadType.h"
#include <array>
#include <glm/mat4x4.hpp>

#ifdef _DEBUG
#include "Logging/ScreenText.h"
#endif

class Camera;
class Currency;
class Editor;
class LevelProperties;
class SeedManager;
class SpreadManager;
class Terrain;
class TerrainCursor;
class ParticleManager;
class Player;
class ResourceManager;
class UIElement;

class Renderer {
public:
    #ifdef _PC
    Renderer(ResourceManager& resourceManager);
    #endif
    Camera* camera_;

    void Render(
        Currency& currency,
        EntityList& entities,
        LevelProperties& levelProperties,
        ParticleManager& particleManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );

    #ifdef _DEBUG
    void RenderEdit(
        EntityList& entities,
        Editor& editor,
        LevelProperties& levelProperties,
        SpreadManager& spreadManager,
        Terrain& terrain,
        TerrainCursor& terrainCursor
    );

    void RenderEditorOnly(
        Editor& editor
    );

    bool DBG_lowResTerrain_;
    #endif

    glm::mat4 GetProjectionMatrix() const { return projectionMatrix_; };

private:
    static constexpr int SHADOW_VIEW = 0;
    static constexpr int TERRAIN_VIEW = 1;
    static constexpr int RENDER_VIEW = 2;
    static constexpr int TRANSPARENCY_VIEW = 3;
    static constexpr int POSTROCESS_VIEW = 4;
    static constexpr int UI_VIEW = 5;

    int width_;
    int height_;
    int renderWidth_;
    int renderHeight_;

    Texture* renderBufferTextures_[2];
    Texture* terrainMapTexture_;
    Texture* shadowBufferTexture_;
    Texture* postProcessTexture_;
    #ifdef _DEBUG
    Texture* DBG_terrainMapTextureLow_;
    #endif

    Mesh* quad_;
    Model* terrainLODs_;
    Mesh* terrainCursor_;

    Shader* uiBarShader_;

    Material blitMaterial_;
    Material postProcessMaterial_;
    Material textMaterial_;
    Material terrainCursorMaterial_;

    #ifdef _DEBUG
    Shader* selectedShader_;
    Shader* selectedFrontShader_;
    Shader* selectedUnshadedShader_;
    #endif

    glm::vec3 lightDirection_;
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::mat4 shadowViewMatrix_;
    glm::mat4 shadowProjectionMatrix_;
    glm::mat4 shadowMatrix_;

    void InitShadowBuffer(Texture* shadowBufferTexture);
    void InitRenderBuffer(Texture* renderColorTexture, Texture* renderDepthTexture);
    void InitPostProcessBuffer(Texture* postProcessTexture);
    void InitUIBuffer();

    FrameBufferHandle backBuffer_;
    FrameBufferHandle shadowBuffer_;
    FrameBufferHandle renderBuffer_;
    FrameBufferHandle postProcessBuffer_;

    TextureSamplerHandle samplers_[Material::MAX_TEXTURES];
    TextureSamplerHandle shadowSampler_;
    TextureSamplerHandle terrainMapSampler_;

    UniformHandle u_shadowUp_;
    UniformHandle u_shadowRight_;
    UniformHandle u_shadowMatrix_;
    UniformHandle u_shadowResolution_;
    UniformHandle u_pose_;
    UniformHandle u_mProps_;
    UniformHandle u_particleProps_;
    UniformHandle u_normalMult_;
    UniformHandle u_lightDirection_;
    UniformHandle u_time_;
    UniformHandle u_cameraPosition_;
    UniformHandle u_cameraUp_;
    UniformHandle u_cameraRight_;
    UniformHandle u_randomVec_;
    UniformHandle u_terrainMeshOffset_;
    UniformHandle u_textProps_;
    UniformHandle u_textCount_;
    UniformHandle u_uiElement_;

    UniformHandle u_dynamicTerrainBubbles_;

    glm::mat4 GetModelViewProjection(const glm::mat4& modelMatrix);
    void SetTexturesFromMaterial(Material* material, bool shadowMap = true);
    void SetLightDirection(const glm::vec3& direction);

    void StartFrame();
    void Clear();
    #ifndef _DEBUG
    void RenderMesh(
        Mesh* mesh, 
        Material* material, 
        InstanceBufferHandle* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr,
    );
    #else
    enum DebugShaderType {
        DS_Default,
        DS_Selected,
        DS_SelectedUnshaded,
        DS_SelectedFront
    };
    void RenderMesh(
        Mesh* mesh, 
        Material* material, 
        InstanceBufferHandle* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr,
        int defaultView = RENDER_VIEW,
        DebugShaderType debugShader = DS_Default
    );
    void RenderEditor(Editor& editor);
    void RenderTerrainCursor(TerrainCursor& terrainCursor);
    void RenderStaticTerrainModifiers(
        Terrain& terrain
    );
    void RenderScreenText();
    #endif
    void RenderTerrain(
        Terrain& terrain, 
        Material* material,
        float maxRadius
    );
    void RenderEntities(
        EntityList& entities
    );
    void RenderSpread(
        SpreadManager& spreadManager, 
        Model* model, 
        std::array<Material, Model::MAX_MESHES_PER_MODEL> materials[SpreadType_Num]
    );
    void RenderSeed(SeedManager& seedManagerm, Material* material);
    void RenderParticles(ParticleManager& particleManager);
    void RenderPostProcess();
    void RenderBlit();
    void RenderUI(Currency& currency);
    void RenderUIElement(UIElement& element, Shader* shader);
    void RenderText(Text& text);
    void PresentFrame();
};


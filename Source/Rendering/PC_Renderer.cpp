#include "Renderer.h"
#include "PC_VertexTypes.h"
#include "ShadowConstants.h"
#include "Components/MeterComponent.h"
#include "Components/SkeletonComponent.h"
#include "Components/StaticModelComponent.h"
#include "Components/TransformComponent.h"
#include "Particle/ParticleManager.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
#include "Camera/Camera.h"
#include "Resource/ResourceManager.h"
#include "Editor/Editor.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>

using namespace glm;

const int SHADOW_VIEW = 0;
const int RENDER_VIEW = 1;
const int POSTROCESS_VIEW = 2;
const int UI_VIEW = 3;

Renderer::Renderer(ResourceManager& resourceManager) {
    renderWidth_ = 1920;
    renderHeight_ = 1080;
    width_ = 1280;
    height_ = 720;

    projectionMatrix_ = perspectiveFovRH_ZO(radians(70.0f), (float)renderWidth_, (float)renderHeight_, 0.5f, 1000.0f);
    shadowProjectionMatrix_ = orthoRH_ZO(
        -ShadowConstants::SHADOW_RANGE, 
        ShadowConstants::SHADOW_RANGE, 
        -ShadowConstants::SHADOW_RANGE, 
        ShadowConstants::SHADOW_RANGE, 
        0.5f, 
        ShadowConstants::SHADOW_DISTANCE
    );

    for (int i = 0; i < Material::MAX_TEXTURES_PER_MATERIAL; i++) {
        std::string samplerName = "s_sampler" + std::to_string(i);
        samplers_[i] = bgfx::createUniform(samplerName.c_str(), bgfx::UniformType::Sampler);
    }
    shadowSampler_ = bgfx::createUniform("s_samplerShadow", bgfx::UniformType::Sampler);
    terrainMapSampler_ = bgfx::createUniform("s_samplerTerrainMap", bgfx::UniformType::Sampler);

    u_shadowMatrix_ = bgfx::createUniform("u_shadowMatrix", bgfx::UniformType::Mat4);
    u_shadowResolution_ = bgfx::createUniform("u_shadowResolution", bgfx::UniformType::Vec4);
    u_shadowUp_ = bgfx::createUniform("u_shadowUp", bgfx::UniformType::Vec4);
    u_shadowRight_ = bgfx::createUniform("u_shadowRight", bgfx::UniformType::Vec4);
    u_pose_ = bgfx::createUniform("u_pose", bgfx::UniformType::Mat4, Bone::MAX_BONES);
    u_materialProps_ = bgfx::createUniform("u_materialProps", bgfx::UniformType::Mat4);
    u_particleProps_ = bgfx::createUniform("u_particleProps", bgfx::UniformType::Mat4);
    u_normalMult_ = bgfx::createUniform("u_normalMult", bgfx::UniformType::Vec4);
    u_lightDirection_ = bgfx::createUniform("u_lightDirection", bgfx::UniformType::Vec4);
    u_timeResolution_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    u_cameraPosition_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    u_cameraUp_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    u_cameraRight_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);
    u_randomVec_ = bgfx::createUniform("u_randomVec", bgfx::UniformType::Vec4);
    u_meter_ = bgfx::createUniform("u_meter", bgfx::UniformType::Vec4);
    u_terrainMeshOffset_= bgfx::createUniform("u_terrainMeshOffset", bgfx::UniformType::Vec4);
    u_textProps_ = bgfx::createUniform("u_textProps", bgfx::UniformType::Mat4);
    DEBUGLOG(bgfx::getCaps()->limits.maxUniforms);
    SetLightDirection(vec3(1.0f, -1.0f, 1.0f));

    backBuffer_ = BGFX_INVALID_HANDLE;

    InitShadowBuffer(resourceManager.GetTexture("t_shadowmap"));
    InitRenderBuffer(resourceManager.GetTexture("t_render_c"), resourceManager.GetTexture("t_render_d"));
    InitPostProcessBuffer(resourceManager.GetTexture("t_post_c"));
    InitUIBuffer();

    terrainMapTexture_ = resourceManager.GetTexture("t_terrainmap");
    terrain_ = &resourceManager.GetModel("st_terrainsheet")->meshes[0];
    quad_ = &resourceManager.GetModel("st_quad")->meshes[0];
    barMaterial_ = resourceManager.GetMaterial("m_uibar");
    blitMaterial_ = resourceManager.GetMaterial("m_preuiblit");
    postProcessMaterial_ = resourceManager.GetMaterial("m_postprocess");
    textMaterial_ = resourceManager.GetMaterial("m_text");
}

// TODO: InitBuffer functions into one generic function
void Renderer::InitShadowBuffer(Texture* shadowBufferTexture) {
    shadowBufferTexture_ = shadowBufferTexture;
    shadowBuffer_ = bgfx::createFrameBuffer(1, &shadowBufferTexture->handle);

    bgfx::setViewFrameBuffer(SHADOW_VIEW, shadowBuffer_);
    bgfx::setViewClear(SHADOW_VIEW, BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(SHADOW_VIEW, 0, 0, ShadowConstants::SHADOW_RESOLUTION, ShadowConstants::SHADOW_RESOLUTION);

    vec4 shadowResolution = vec4(ShadowConstants::SHADOW_RESOLUTION, ShadowConstants::SHADOW_RESOLUTION, 0.0f, 0.0f);
    bgfx::setUniform(u_shadowResolution_, &shadowResolution);
}

void Renderer::InitRenderBuffer(Texture* renderColorTexture, Texture* renderDepthTexture) {
    renderBufferTextures_[0] = renderColorTexture;
    renderBufferTextures_[1] = renderDepthTexture;
    TextureHandle handles[] = { renderBufferTextures_[0]->handle, renderBufferTextures_[1]->handle };
    renderBuffer_ = bgfx::createFrameBuffer(2, handles);

    bgfx::setViewFrameBuffer(RENDER_VIEW, renderBuffer_);
    bgfx::setViewClear(RENDER_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(RENDER_VIEW, 0, 0, renderWidth_, renderHeight_);
}

void Renderer::InitPostProcessBuffer(Texture* postProcessTexture) {
    postProcessTexture_ = postProcessTexture;
    postProcessBuffer_ = bgfx::createFrameBuffer(1, &postProcessTexture->handle);
    bgfx::setViewFrameBuffer(POSTROCESS_VIEW, postProcessBuffer_);
    bgfx::setViewClear(POSTROCESS_VIEW, BGFX_CLEAR_COLOR, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(POSTROCESS_VIEW, 0, 0, renderWidth_, renderHeight_);
}

void Renderer::InitUIBuffer() {
    bgfx::setViewFrameBuffer(UI_VIEW, backBuffer_);
    bgfx::setViewClear(UI_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(UI_VIEW, 0, 0, width_, height_);
    bgfx::setViewMode(UI_VIEW, bgfx::ViewMode::Sequential);
}

void Renderer::StartFrame() {
    viewMatrix_ = camera_->GetViewMatrix();
    bgfx::setViewTransform(RENDER_VIEW, &viewMatrix_, &projectionMatrix_);

    vec3 forward = camera_->transform_.GetForwardVector();
    forward.y = 0.0f;
    forward = normalize(forward);

    vec3 cameraPos = camera_->transform_.position;
    cameraPos.y = 0.0f;

    vec3 lookPosition = cameraPos + forward * ShadowConstants::SHADOW_FORWARD;
    vec3 lightPosition = -lightDirection_ * ShadowConstants::SHADOW_DISTANCE * 0.75f + lookPosition;
    shadowViewMatrix_ = lookAtRH(lightPosition, lookPosition, Transform::worldUp);
    shadowMatrix_ = shadowProjectionMatrix_ * shadowViewMatrix_;
    bgfx::setViewTransform(SHADOW_VIEW, &shadowViewMatrix_, &shadowProjectionMatrix_);

    vec4 timeResolution = vec4(GlobalTime::GetTime(), width_, height_, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);
    vec4 randomVec = vec4(rand());

    bgfx::setUniform(u_timeResolution_, &timeResolution);
    bgfx::setUniform(u_cameraPosition_, &cameraPosition);
    bgfx::setUniform(u_cameraUp_, &cameraUp);
    bgfx::setUniform(u_cameraRight_, &cameraRight);
    bgfx::setUniform(u_randomVec_, &randomVec);
    bgfx::setUniform(u_shadowMatrix_, &shadowMatrix_);

    bgfx::touch(0);
}

void Renderer::Clear() {
    return;
}

void Renderer::PresentFrame() {
    bgfx::frame(); 
}

void Renderer::RenderMesh(
    Mesh* mesh, 
    Material* material, 
    InstanceBufferHandle* instanceBuffer, 
    glm::mat4* modelMatrix,
    GPUPose* pose 
) {
    vec4 normalMult;

    int numOfRenders = 1;
    if (material->castShadows)
        numOfRenders *= 2;
    if (material->triangleType > ONE_SIDED)
        numOfRenders *= 2;

    int curPass = 0;
    int curFace = 0;

    mat4 transposeProps = transpose(material->properties);

    for (int n = 0; n < numOfRenders; n++) {
        bgfx::setUniform(u_materialProps_, &transposeProps);
        if (pose != nullptr)
            bgfx::setUniform(u_pose_, pose->data(), Bone::MAX_BONES);
        if (modelMatrix != nullptr)
            bgfx::setTransform(modelMatrix);

        if (curFace == 1) {
            bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_FRONT_CCW);
            normalMult.x = material->triangleType == TWO_SIDED_NEGATIVE_BACK ? -1.0f : 1.0f;
            normalMult.y = -1.0f;
        }
        else {
            normalMult.x = 1.0f;
            normalMult.y = 1.0f;
        }
        bgfx::setUniform(u_normalMult_, &normalMult);

        int view;
        MaterialShaderHandle shaderHandle;
        if (curPass == 0) {
            view = RENDER_VIEW;
            shaderHandle = material->shaderHandle;
            SetTexturesFromMaterial(material, true);
        }
        else {
            view = SHADOW_VIEW;
            shaderHandle = material->shadowShaderHandle;
            SetTexturesFromMaterial(material, false);
        }

        if (instanceBuffer != nullptr)
            bgfx::setInstanceDataBuffer(instanceBuffer);

        bgfx::setVertexBuffer(0, mesh->vertexBuffer);
        bgfx::setIndexBuffer(mesh->indexBuffer);
        bgfx::submit(view, shaderHandle);

        if (material->triangleType > ONE_SIDED) {
            curFace++;

            // When two faces have been rendered,
            // render to the next pass
            if (curFace > 1) {
                curFace = 0;
                curPass = 1; 
            }
        }
        else
            curPass++;
    }
}

void Renderer::RenderTerrain(Terrain& terrain, Material* material) {
    int radius = terrain.blobProperties_.maxRadius / Terrain::TERRAIN_MESH_SIZE;
    radius += 1;
    for (int x = -radius; x < radius; x++)
    for (int y = -radius; y < radius; y++) { 
        vec4 offset = vec4(x * Terrain::TERRAIN_MESH_SIZE, 0.0f, y * Terrain::TERRAIN_MESH_SIZE, 0.0f);
        bgfx::setUniform(u_terrainMeshOffset_, &offset);

        bgfx::setTexture(Material::TERRAIN_MAP_TEXINDEX, terrainMapSampler_, terrainMapTexture_->handle);
        RenderMesh(terrain_, material);
    };
}

EntityKey constexpr key = GetEntityKey<StaticModelComponent, TransformComponent>();
EntityKey constexpr skeletonKey = GetEntityKey<SkeletonComponent>();
void Renderer::RenderEntities(
    EntityList& entities, 
    ComponentList& components
) {
    auto& meterComponent = components.Get<MeterComponent>();
    auto& skeletonComponent = components.Get<SkeletonComponent>();
    auto& staticModelComponent = components.Get<StaticModelComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    GPUPose pose;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec4 meter = vec4(meterComponent.meter[i], meterComponent.maxMeter[i], 0.0f, 0.0f); 
        bgfx::setUniform(u_meter_, &meter);
        mat4 modelMatrix = transformComponent.renderTransform[i].ToMatrix();

        Model& model = *staticModelComponent.model[i];
        bool skeletal = false;
        if (entity.MatchesKey(skeletonKey)) {
            skeletal = true; 
            skeletonComponent.skeleton[i]->PoseToGPUPose(
                pose,
                skeletonComponent.renderPose[i]
            );
        }
        for (int m = 0; m < model.meshes.size(); m++) {
            #ifdef _DEBUG
            Material* material = entity.DBG_selected ?
                staticModelComponent.selectedMaterials[i][m] :
                staticModelComponent.materials[i][m];
            #else
            Material* material = staticModelComponent.materials[i][m];
            #endif
            Mesh* mesh = &model.meshes[m];
            RenderMesh(mesh, material, nullptr, &modelMatrix, skeletal ? &pose: nullptr);
        }
    }
}

void Renderer::RenderSpread(
    SpreadManager& spreadManager, 
    Model* model,
    vector_const<Material*, Model::MAX_MESHES_PER_MODEL>& materials
) {
    uint32_t count = spreadManager.GetCount();
    if (count == 0)
        return;

    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(SpreadRenderData));
    memcpy(instanceBuffer.data, spreadManager.GetRenderData(), sizeof(SpreadRenderData) * count);

    for (int m = 0; m < model->meshes.size(); m++) {
        Mesh* mesh = &model->meshes[m];
        Material* material = materials[m];
        RenderMesh(mesh, material, &instanceBuffer);
    }
}

void Renderer::RenderSeed(SeedManager& seedManager, Material* material) {
    uint32_t count = seedManager.seeds_.size();
    if (count == 0)
        return;
    
    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, seedManager.positions_.data(), sizeof(vec4) * count);

    RenderMesh(quad_, material, &instanceBuffer);
}

void Renderer::RenderParticles(ParticleManager& particleManager) {
    mat4 particleProps;
    mat4 modelMatrix;
    for (int i = 0; i < MAX_EMITTERS; i++) {
        ParticleEmitter& emitter = particleManager.emitters_[i];
        if (!emitter.alive_)
            continue;

        if (emitter.particles_.size() == 0)
            continue;

        particleProps[0] = emitter.properties_->startColor;
        particleProps[1] = emitter.properties_->endColor;
        particleProps = transpose(particleProps);
        bgfx::setUniform(u_particleProps_, &particleProps);

        modelMatrix = mat4(1.0f);
        if (emitter.properties_->localSpace)
            modelMatrix = translate(modelMatrix, emitter.transform_.position);
        bgfx::setTransform(&modelMatrix);

        bgfx::InstanceDataBuffer instanceBuffer;
        bgfx::allocInstanceDataBuffer(&instanceBuffer, emitter.particles_.size(), sizeof(Particle));
        memcpy(instanceBuffer.data, emitter.particles_.data(), sizeof(Particle) * emitter.particles_.size());
        RenderMesh(quad_, emitter.properties_->material, &instanceBuffer);
    }
}

void Renderer::RenderPostProcess() {
    SetTexturesFromMaterial(postProcessMaterial_, false);
    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(POSTROCESS_VIEW, postProcessMaterial_->shaderHandle);
}

void Renderer::RenderBlit() {
    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB);
    bgfx::setTexture(0, samplers_[0], blitMaterial_->textures[0]->handle);
    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, blitMaterial_->shaderHandle);
}

void Renderer::RenderUI(ComponentList& components) {
    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB);
    auto& meterComponent = components.Get<MeterComponent>();

    vec4 meter = vec4(meterComponent.meter[PLAYER_ENTITY], meterComponent.maxMeter[PLAYER_ENTITY], 0.0f, 0.0f); 
    bgfx::setUniform(u_meter_, &meter);

    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, barMaterial_->shaderHandle);
}

void Renderer::RenderText(Text& text) {
    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
    uint32_t count = text.length_;
    if (count == 0)
        return;
    
    mat4 textProps;
    textProps[0][0] = text.properties_.position.x;
    textProps[0][1] = text.properties_.position.y;
    textProps[0][2] = text.properties_.scale;
    textProps[0][3] = text.properties_.kerning;
    textProps[1][0] = text.properties_.hAlignment;
    textProps[1][1] = text.properties_.vAlignment;
    textProps[1][2] = text.properties_.hAnchor;
    textProps[1][3] = text.properties_.vAnchor;
    textProps[2][0] = (float)text.length_;
    textProps = transpose(textProps);

    bgfx::setUniform(u_textProps_, &textProps);
    
    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, text.glyphs_.data(), sizeof(vec4) * count);
    bgfx::setInstanceDataBuffer(&instanceBuffer);

    // TODO: Move text material to Text class?
    bgfx::setTexture(0, samplers_[0], textMaterial_->textures[0]->handle);

    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, textMaterial_->shaderHandle);
}

void Renderer::RenderScreenText() {
    if (!ScreenText::IsEnabled())
        return;

    for (Text& text : ScreenText::GetText()) {
        RenderText(text);
    }
}

void Renderer::RenderEditor(Editor& editor) {
    RenderText(editor.modeText_);
    RenderText(editor.target_.text_);
    RenderText(editor.textInput_.text_);
}

void Renderer::SetTexturesFromMaterial(Material* material, bool shadowMap) {
    for (int i = 0; i < material->textures.size(); i++)
        bgfx::setTexture(i, samplers_[i], material->textures[i]->handle);

    if (shadowMap)
        bgfx::setTexture(Material::SHADOW_TEXINDEX, shadowSampler_, shadowBufferTexture_->handle);
}

void Renderer::SetLightDirection(const vec3& direction) {
    quat directionQuat = quatLookAtRH(normalize(direction), Transform::worldUp);
    lightDirection_ = rotate(directionQuat, Transform::worldForward);
    vec4 shadowRight = vec4(rotate(directionQuat, Transform::worldRight), 0.0f);
    vec4 shadowUp = vec4(rotate(directionQuat, Transform::worldUp), 0.0f);

    vec4 uniformDirection = normalize(vec4(lightDirection_, 0.0f));
    bgfx::setUniform(u_lightDirection_, &uniformDirection);
    bgfx::setUniform(u_shadowRight_, &shadowRight);
    bgfx::setUniform(u_shadowUp_, &shadowUp);
}

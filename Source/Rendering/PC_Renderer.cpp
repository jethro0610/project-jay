#include "Renderer.h"
#include "PC_VertexTypes.h"
#include "ShadowConstants.h"
#include "Particle/ParticleManager.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
#include "Camera/Camera.h"
#include "Resource/ResourceManager.h"
#include "Editor/Editor.h"
#include "Entity/Player.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>

using namespace glm;

Renderer::Renderer(ResourceManager& resourceManager) {
    renderWidth_ = 1920;
    renderHeight_ = 1080;
    width_ = 1280;
    height_ = 720;

    projectionMatrix_ = perspectiveFovRH_ZO(radians(60.0f), (float)renderWidth_, (float)renderHeight_, 0.5f, 4096.0f);
    shadowProjectionMatrix_ = orthoRH_ZO(
        -ShadowConstants::SHADOW_RANGE, 
        ShadowConstants::SHADOW_RANGE, 
        -ShadowConstants::SHADOW_RANGE, 
        ShadowConstants::SHADOW_RANGE, 
        0.5f, 
        ShadowConstants::SHADOW_DISTANCE
    );

    for (int i = 0; i < Material::MAX_TEXTURES; i++) {
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
    u_mProps_ = bgfx::createUniform("u_mProps", bgfx::UniformType::Vec4, 4);
    u_particleProps_ = bgfx::createUniform("u_particleProps", bgfx::UniformType::Mat4);
    u_normalMult_ = bgfx::createUniform("u_normalMult", bgfx::UniformType::Vec4);
    u_lightDirection_ = bgfx::createUniform("u_lightDirection", bgfx::UniformType::Vec4);
    u_time_ = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
    u_cameraPosition_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    u_cameraUp_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    u_cameraRight_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);
    u_randomVec_ = bgfx::createUniform("u_randomVec", bgfx::UniformType::Vec4);
    u_meter_ = bgfx::createUniform("u_meter", bgfx::UniformType::Vec4);
    u_terrainMeshOffset_= bgfx::createUniform("u_terrainMeshOffset", bgfx::UniformType::Vec4);
    u_textProps_ = bgfx::createUniform("u_textProps", bgfx::UniformType::Mat4);
    SetLightDirection(normalize(vec3(0.75f, -1.0f, 0.75f)));

    backBuffer_ = BGFX_INVALID_HANDLE;

    InitShadowBuffer(resourceManager.GetTexture("t_shadowmap"));
    InitRenderBuffer(resourceManager.GetTexture("t_render_c"), resourceManager.GetTexture("t_render_d"));
    InitPostProcessBuffer(resourceManager.GetTexture("t_post_c"));
    InitUIBuffer();

    terrainMapTexture_ = resourceManager.GetTexture("t_terrainmap");
    #ifdef _DEBUG
    DBG_terrainMapTextureLow_ = resourceManager.GetTexture("t_terrainmap_low");
    #endif

    terrainLODs_ = resourceManager.GetModel("st_terrainsheet");
    terrainCursor_ = &resourceManager.GetModel("st_terraincursor")->meshes[0];
    quad_ = &resourceManager.GetModel("st_quad")->meshes[0];

    barMaterial_.shader = resourceManager.GetShader("vs_uibar", "fs_uibar");
    coverageBarMaterial_.shader = resourceManager.GetShader("vs_coveragebar", "fs_coveragebar");

    blitMaterial_.shader = resourceManager.GetShader("vs_screenquad", "fs_blit");
    blitMaterial_.numTextures = 1;
    blitMaterial_.textures[0] = resourceManager.GetTexture("t_post_c");

    postProcessMaterial_.shader = resourceManager.GetShader("vs_screenquad", "fs_postprocess");
    postProcessMaterial_.numTextures = 1;
    postProcessMaterial_.textures[0] = resourceManager.GetTexture("t_render_c");

    textMaterial_.shader = resourceManager.GetShader("vs_glyph", "fs_text");
    textMaterial_.numTextures = 1;
    textMaterial_.textures[0] = resourceManager.GetTexture("t_font");

    terrainCursorMaterial_.shader = resourceManager.GetShader("vs_terraincursor", "fs_terraincursor");
    terrainCursorMaterial_.numTextures = 0;
    terrainCursorMaterial_.castShadows = false;

    #ifdef _DEBUG
    selectedShader_ = resourceManager.GetShader("vs_static", "fs_selected");
    selectedUnshadedShader_ = resourceManager.GetShader("vs_static", "fs_selected_unshaded");
    selectedFrontShader_ = resourceManager.GetShader("vs_static", "fs_selected_front");
    persistShader_ = resourceManager.GetShader("vs_static", "fs_persist");
    #endif
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

    bgfx::setViewFrameBuffer(TERRAIN_VIEW, renderBuffer_);
    bgfx::setViewRect(TERRAIN_VIEW, 0, 0, renderWidth_, renderHeight_);
    bgfx::setViewClear(TERRAIN_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);

    bgfx::setViewFrameBuffer(RENDER_VIEW, renderBuffer_);
    bgfx::setViewRect(RENDER_VIEW, 0, 0, renderWidth_, renderHeight_);

    bgfx::setViewFrameBuffer(TRANSPARENCY_VIEW, renderBuffer_);
    bgfx::setViewRect(TRANSPARENCY_VIEW, 0, 0, renderWidth_, renderHeight_);
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
    bgfx::setViewTransform(TERRAIN_VIEW, &viewMatrix_, &projectionMatrix_);
    bgfx::setViewTransform(RENDER_VIEW, &viewMatrix_, &projectionMatrix_);
    bgfx::setViewTransform(TRANSPARENCY_VIEW, &viewMatrix_, &projectionMatrix_);

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

    vec4 time = vec4(GlobalTime::GetTime(), 0.0f, 0.0f, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);
    vec4 randomVec = vec4(rand());

    bgfx::setUniform(u_time_, &time);
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
    GPUPose* pose,
    int defaultView,
    DebugShaderType debugShaderType 
) {
    vec4 normalMult;

    int numOfRenders = 1;
    if (material->castShadows)
        numOfRenders *= 2;
    if (material->triangleType > ONE_SIDED)
        numOfRenders *= 2;

    int curPass = 0;
    int curFace = 0;
    
    for (int n = 0; n < numOfRenders; n++) {
        uint64_t state = BGFX_STATE_DEFAULT;
        int view = defaultView;
        switch (material->transparencyType) {
            case TRANSPARENCY_UNORDERED:
                state = state | BGFX_STATE_BLEND_ALPHA;
                break;

            case TRANSPARENCY_ADDITIVE:
                state = state & ~BGFX_STATE_WRITE_Z;
                state = state | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
                state = state | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
                view = TRANSPARENCY_VIEW;
                break;

            case TRANSPARENCY_ADDITIVE_BRIGHT:
                state = state & ~BGFX_STATE_WRITE_Z;
                state = state | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_ONE);
                state = state | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
                view = TRANSPARENCY_VIEW;
                break;

            default:
                break;
        }


        bgfx::setUniform(u_mProps_, &material->properties, 4);
        if (pose != nullptr)
            bgfx::setUniform(u_pose_, pose->data(), Bone::MAX_BONES);
        if (modelMatrix != nullptr)
            bgfx::setTransform(modelMatrix);

        if (curFace == 1) {
            state = state & ~BGFX_STATE_CULL_CW;
            state = state | BGFX_STATE_CULL_CCW;
            normalMult.x = material->triangleType == TWO_SIDED_NEGATIVE_BACK ? -1.0f : 1.0f;
            normalMult.y = -1.0f;
        }
        else {
            normalMult.x = 1.0f;
            normalMult.y = 1.0f;
        }
        bgfx::setUniform(u_normalMult_, &normalMult);

        bgfx::ProgramHandle shaderHandle;
        if (curPass == 0) {
            #ifndef _DEBUG
            shaderHandle = material->shader->handle;
            #else
            switch (debugShaderType) {
                case DS_Default:
                    shaderHandle = material->shader->handle;
                    break;

                case DS_Selected:
                    shaderHandle = selectedShader_->handle;
                    break;

                case DS_SelectedUnshaded:
                    shaderHandle = selectedUnshadedShader_->handle;
                    break;

                case DS_SelectedFront:
                    shaderHandle = selectedFrontShader_->handle;
                    break;

                case DS_Persist:
                    shaderHandle = persistShader_->handle;
                    break;
            }
            #endif
            SetTexturesFromMaterial(material, true);
        }
        else {
            view = SHADOW_VIEW;
            shaderHandle = material->shadowShader->handle;
            SetTexturesFromMaterial(material, false);
        }

        if (instanceBuffer != nullptr)
            bgfx::setInstanceDataBuffer(instanceBuffer);

        bgfx::setState(state);
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

void Renderer::RenderTerrain(Terrain& terrain, Material* material, float maxRadius) {
    const float MAX_DIST_TO_CAMERA = 6144.0f;
    int radius = maxRadius / TerrainConsts::MESH_SIZE;
    radius += 1;

    std::vector<vec4> offsets[TerrainConsts::NUM_LODS];
    for (int i = 0; i < TerrainConsts::NUM_LODS; i++) {
        offsets[i].reserve(4096);
    }

    vec3 cameraForward = camera_->transform_.GetForwardVector();
    cameraForward.y = 0.0f;
    cameraForward = normalize(cameraForward);
    vec3 cameraPlanar = camera_->transform_.position - cameraForward * TerrainConsts::MESH_SIZE * 2.0f;
    cameraPlanar.y = 0.0f;

    for (int x = -radius; x < radius; x++)
    for (int z = -radius; z < radius; z++) { 
        vec3 offset = vec3(x * TerrainConsts::MESH_SIZE, 0.0f, z * TerrainConsts::MESH_SIZE);
        vec3 vectorToOffset = normalize(offset - cameraPlanar);
        float dot = glm::dot(cameraForward, vectorToOffset);
        if (dot < 0.5f)
            continue;

        float distanceToCamera = distance(cameraPlanar, offset);
        if (distanceToCamera >= MAX_DIST_TO_CAMERA)
            continue;

        int lodLevel = TerrainConsts::NUM_LODS * std::pow(distanceToCamera / MAX_DIST_TO_CAMERA, 0.75f);
        offsets[lodLevel].push_back(vec4(offset, 0.0f));
    };

    for (int i = 0; i < TerrainConsts::NUM_LODS; i++) {
        if (offsets[i].size() > 0) {
            bgfx::InstanceDataBuffer instanceBuffer;
            bgfx::allocInstanceDataBuffer(&instanceBuffer, offsets[i].size(), sizeof(vec4));
            memcpy(instanceBuffer.data, offsets[i].data(), sizeof(vec4) * offsets[i].size());
            RenderMesh(&terrainLODs_->meshes[i], material, &instanceBuffer, nullptr, nullptr, TERRAIN_VIEW);
        }
    }
}


void Renderer::RenderEntitiesS(
    EntityList& entities
) {
    GPUPose pose;
    mat4 matrix;

    for (int i = 0; i < 128; i++) {
        if (!entities[i].alive_) continue;
        Entity& entity = entities[i];
        matrix = entity.renderTransform_.ToMatrix();

        bool skeletal = entity.GetFlag(Entity::EF_UseSkeleton);
        if (skeletal) {
            entity.skeleton_->PoseToGPUPose(
                pose,
                entity.renderPose_
            );
        }

        Model* model = entity.model_;
        for (int m = 0; m < model->meshes.size(); m++) {
            Material* material = &entity.materials_[m];
            Mesh* mesh = &model->meshes[m];

            #ifndef _DEBUG
            RenderMesh(mesh, material, nullptr, &matrix, skeletal ? &pose : nullptr);
            #else
            DebugShaderType debugShaderType = DS_Default;
            if (entity.DBG_selected_)
                debugShaderType = DS_Selected;
            else if (entity.DBG_persistView_)
                debugShaderType = DS_Persist;
            RenderMesh(mesh, material, nullptr, &matrix, skeletal ? &pose : nullptr, RENDER_VIEW, debugShaderType);
            #endif 
        }
    }
}

void Renderer::RenderSpread(
    SpreadManager& spreadManager, 
    Model* model,
    std::array<Material, Model::MAX_MESHES_PER_MODEL>& materials
) {
    uint32_t count = spreadManager.GetSpreadCount();
    if (count == 0)
        return;

    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(SpreadManager::RenderData));
    memcpy(instanceBuffer.data, spreadManager.GetSpreadData(), sizeof(SpreadManager::RenderData) * count);

    for (int m = 0; m < model->meshes.size(); m++) {
        Mesh* mesh = &model->meshes[m];
        Material* material = &materials[m];
        RenderMesh(mesh, material, &instanceBuffer);
    }
}

void Renderer::RenderWeed(
    SpreadManager& spreadManager, 
    Model* model,
    std::array<Material, Model::MAX_MESHES_PER_MODEL>& materials
) {
    uint32_t count = spreadManager.GetWeedCount();
    if (count == 0)
        return;

    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(SpreadManager::RenderData));
    memcpy(instanceBuffer.data, spreadManager.GetWeedData(), sizeof(SpreadManager::RenderData) * count);

    for (int m = 0; m < model->meshes.size(); m++) {
        Mesh* mesh = &model->meshes[m];
        Material* material = &materials[m];
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

        particleProps[0] = emitter.properties_.startColor;
        particleProps[1] = emitter.properties_.endColor;
        particleProps = transpose(particleProps);
        bgfx::setUniform(u_particleProps_, &particleProps);

        modelMatrix = mat4(1.0f);
        if (emitter.properties_.localSpace)
            modelMatrix = translate(modelMatrix, emitter.transform_.position);
        bgfx::setTransform(&modelMatrix);

        bgfx::InstanceDataBuffer instanceBuffer;
        bgfx::allocInstanceDataBuffer(&instanceBuffer, emitter.particles_.size(), sizeof(Particle));
        memcpy(instanceBuffer.data, emitter.particles_.data(), sizeof(Particle) * emitter.particles_.size());
        RenderMesh(quad_, &emitter.properties_.material, &instanceBuffer);
    }
}

void Renderer::RenderPostProcess() {
    SetTexturesFromMaterial(&postProcessMaterial_, false);
    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(POSTROCESS_VIEW, postProcessMaterial_.shader->handle);
}

void Renderer::RenderBlit() {
    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB);
    bgfx::setTexture(0, samplers_[0], blitMaterial_.textures[0]->handle);
    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, blitMaterial_.shader->handle);
}

void Renderer::RenderUI(EntityList& entities, SpreadManager& spreadManager) {
    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB);

    Player* player = (Player*)&entities[0];

    vec4 meter = vec4(player->meter_, 0.0f, 0.0f, 0.0f); 
    bgfx::setUniform(u_meter_, &meter);

    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, barMaterial_.shader->handle);

    meter = vec4(spreadManager.GetCoverage() / 0.45f, 0.0f, 0.0f, 0.0f); 
    bgfx::setUniform(u_meter_, &meter);

    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, coverageBarMaterial_.shader->handle);

    Text itemText;
    itemText.properties_.scale = 80.0f;
    itemText.properties_.hAlignment = Text::CENTER_ALIGN;
    itemText.properties_.vAlignment = Text::TOP_ALIGN;
    itemText.properties_.hAnchor = Text::CENTER_ALIGN;
    itemText.properties_.vAnchor = Text::TOP_ALIGN;

    switch (player->item_) {
        case Item::None:
            itemText = "";
            break;

        case Item::Boost:
            itemText = "Boost";
            break;

        case Item::Cut:
            itemText = "Cut";
            break;

        case Item::Radius:
            itemText = "Radius";
            break;

        case Item::NumItems:
            itemText = "Cut";
            break;

        default:
            break;
    }
    RenderText(itemText);
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
    bgfx::setTexture(0, samplers_[0], textMaterial_.textures[0]->handle);

    bgfx::setVertexBuffer(0, quad_->vertexBuffer);
    bgfx::setIndexBuffer(quad_->indexBuffer);
    bgfx::submit(UI_VIEW, textMaterial_.shader->handle);
}

void Renderer::RenderScreenText() {
    if (!ScreenText::IsEnabled())
        return;

    for (Text& text : ScreenText::GetText()) {
        RenderText(text);
    }
}

#ifdef _DEBUG
void Renderer::RenderEditor(Editor& editor) {
    RenderText(editor.modeText_);
    RenderText(editor.target_.name_);
    RenderText(editor.textInput_.text_);
    RenderText(editor.notification_.text_);
}

void Renderer::RenderTerrainBubbles(Terrain& terrain) {
    Transform bubbleTransform;
    bubbleTransform.scale = vec3(2.0f, 4.0f, 2.0f);
    mat4 bubbleMatrix;
    for (TerrainBubble& bubble : terrain.DBG_bubbles_) {
        bubbleTransform.position = bubble.position;
        bubbleMatrix = bubbleTransform.ToMatrix();
        RenderMesh(
            &terrain.DBG_nodeModel_->meshes[0],
            &terrain.DBG_bubbleMaterial_,
            nullptr,
            &bubbleMatrix,
            nullptr,
            RENDER_VIEW,
            bubble.DBG_selected_ ? DS_SelectedUnshaded : DS_Default
        );
    }
}

void Renderer::RenderTerrainCurves(Terrain& terrain) {
    Transform curveTransform;
    curveTransform.scale = vec3(2.0f, 4.0f, 2.0f);
    mat4 curveMatrix;
    for (TerrainCurve& curve : terrain.DBG_curves_) {
        for (int i = 0; i < 65; i++) {
            DebugShaderType shaderType = DS_Default;
            if (i == 0 || i == 64) {
                curveTransform.scale = vec3(2.0f, 4.0f, 2.0f);
                if (curve.DBG_selectedPoint_ == (i / 64) * 3)
                    shaderType = DS_SelectedUnshaded;
            }
            else {
                curveTransform.scale = vec3(1.0f, 1.0f, 1.0f);
            }

            vec4 curvePos = curve.GetPosition(i / 64.0f);
            curveTransform.position = curvePos;
            curveMatrix = curveTransform.ToMatrix();
            RenderMesh(
                &terrain.DBG_nodeModel_->meshes[0], 
                &terrain.DBG_curveMaterial_, 
                nullptr,
                &curveMatrix, 
                nullptr, 
                RENDER_VIEW, 
                shaderType
            );
        }

        curveTransform.scale = vec3(2.0f, 4.0f, 2.0f);
        curveTransform.position = curve.points[1];
        curveMatrix = curveTransform.ToMatrix();
        RenderMesh(
            &terrain.DBG_nodeModel_->meshes[0], 
            &terrain.DBG_curveControlMaterial_, 
            nullptr, 
            &curveMatrix, 
            nullptr, 
            RENDER_VIEW,
            curve.DBG_selectedPoint_ == 1 ? DS_SelectedFront : DS_Default
        );
        curveTransform.scale = vec3(1.0f, 1.0f, 1.0f);
        for (int i = 1; i < 32; i++) {
            vec4 pos = mix(curve.points[0], curve.points[1], i / 32.0f);
            curveTransform.position = pos;
            curveMatrix = curveTransform.ToMatrix();
            RenderMesh(
                &terrain.DBG_nodeModel_->meshes[0], 
                &terrain.DBG_curveControlMaterial_, 
                nullptr, 
                &curveMatrix
            );
        }

        curveTransform.scale = vec3(2.0f, 4.0f, 2.0f);
        curveTransform.position = curve.points[2];
        curveMatrix = curveTransform.ToMatrix();
        RenderMesh(
            &terrain.DBG_nodeModel_->meshes[0], 
            &terrain.DBG_curveControlMaterial_, 
            nullptr, 
            &curveMatrix, 
            nullptr, 
            RENDER_VIEW,
            curve.DBG_selectedPoint_ == 2 ? DS_SelectedFront : DS_Default
        );
        curveTransform.scale = vec3(1.0f, 1.0f, 1.0f);
        for (int i = 1; i < 32; i++) {
            vec4 pos = mix(curve.points[2], curve.points[3], i / 32.0f);
            curveTransform.position = pos;
            curveMatrix = curveTransform.ToMatrix();
            RenderMesh(
                &terrain.DBG_nodeModel_->meshes[0], 
                &terrain.DBG_curveControlMaterial_, 
                nullptr, 
                &curveMatrix
            );
        }
    }
}

void Renderer::RenderTerrainNoises(Terrain& terrain) {
    Transform transform;
    transform.scale = vec3(2.0f, 4.0f, 2.0f);
    mat4 matrix;
    for (TerrainNoise& noise : terrain.DBG_noises_) {
        transform.position = vec3(noise.position_.x, terrain.GetHeight(noise.position_), noise.position_.y);
        matrix = transform.ToMatrix();
        RenderMesh(
            &terrain.DBG_nodeModel_->meshes[0],
            &terrain.DBG_noiseMaterial_,
            nullptr,
            &matrix,
            nullptr,
            RENDER_VIEW,
            noise.DBG_selected_ ? DS_SelectedUnshaded : DS_Default
        );
    }
}

void Renderer::RenderTerrainCursor(TerrainCursor& terrainCursor) {
    if (!terrainCursor.visible)
        return;

    Transform cursorTransform;
    cursorTransform.position = terrainCursor.position;
    cursorTransform.scale = vec3(terrainCursor.radius);
    mat4 cursorMatrix = cursorTransform.ToMatrix();
    RenderMesh(terrainCursor_, &terrainCursorMaterial_, nullptr, &cursorMatrix);
}
#endif

void Renderer::SetTexturesFromMaterial(Material* material, bool shadowMap) {
    for (int i = 0; i < material->numTextures; i++)
        bgfx::setTexture(i, samplers_[i], material->textures[i]->handle);


    #ifdef _DEBUG
    if (DBG_lowResTerrain_)
        bgfx::setTexture(Material::TERRAIN_MAP_TEXINDEX, terrainMapSampler_, DBG_terrainMapTextureLow_->handle);
    else 
        bgfx::setTexture(Material::TERRAIN_MAP_TEXINDEX, terrainMapSampler_, terrainMapTexture_->handle);
    #else
        bgfx::setTexture(Material::TERRAIN_MAP_TEXINDEX, terrainMapSampler_, terrainMapTexture_->handle);
    #endif

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

#include "World.h"
#include "../../Helpers/ChunkHelpers.h"
#include "../../Resource/DXResources.h"
#include "../../Resource/ResourceManager.h"

struct QuadInfo {
    int valid;
    int hasQuad[3];
    int forward[4];
};

void World::GenerateNoiseTexture_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    D3D11_MAPPED_SUBRESOURCE noiseTextureResouce;
    context->Map(dxResources.noiseTexture_, 0, D3D11_MAP_WRITE_DISCARD, 0, &noiseTextureResouce);
    float* noiseArray = new float[NOISE_SIZE * NOISE_SIZE];

    for (int x = 0; x < NOISE_SIZE; x++)
    for (int y = 0; y < NOISE_SIZE; y++) {
        float xOffset = float(x) / NOISE_DETAIL - NOISE_OFFSET;
        float yOffset = float(y) / NOISE_DETAIL - NOISE_OFFSET;
        float noiseVal = noise_->GetNoise(xOffset, yOffset);
        *(noiseArray + y * NOISE_SIZE + x) = noiseVal;
    }

    memcpy(noiseTextureResouce.pData, noiseArray, NOISE_SIZE * NOISE_SIZE * sizeof(float));
    context->Unmap(dxResources.noiseTexture_, 0);
    delete[] noiseArray;
}

void World::GenerateMeshGPU_P(ivec3 chunk) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;
    vec4 chunkPos = vec4(vec3(chunk) * CHUNK_SIZE, 0.0f);
    chunk = GetNormalizedChunk(chunk);
    WorldMesh& chunkMesh = dxResources.worldMeshes_[chunk.x][chunk.y][chunk.z];

    PerChunkData chunkData = { chunkPos, terrainModifiers_.GetCount() };
    dxResources.UpdateBuffer(dxResources.perChunkCBuffer_, &chunkData, sizeof(PerChunkData));

    ID3D11ShaderResourceView* resources[2] = { dxResources.noiseTextureSRV_, dxResources.terrainModSRV_ };
    context->CSSetShaderResources(0, 2, resources);
    context->CSSetSamplers(0, 1, &dxResources.textureSampler_);
    UINT zeroes[4] = {0, 0, 0, 0};

    context->CSSetShader(dxResources.csWorldVertex_, nullptr, 0);
    ID3D11UnorderedAccessView* vertView[2] = {
        dxResources.csWorldVertexView_, 
        dxResources.csWorldCountView_
    };
    context->CSSetUnorderedAccessViews(0, 2, vertView, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.csWorldVertexView_, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.csWorldCountView_, zeroes);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    context->CopyResource(chunkMesh.vertexBuffer, dxResources.csWorldVertexBuffer_);

    context->CopyResource(dxResources.csWorldCountOutput_, dxResources.csWorldCountBuffer_);
    D3D11_MAPPED_SUBRESOURCE countResource;
    context->Map(dxResources.csWorldCountOutput_, 0, D3D11_MAP_READ, 0, &countResource);
    chunkMesh.vertexCount = reinterpret_cast<uint*>(countResource.pData)[0];
    context->Unmap(dxResources.csWorldCountOutput_, 0);
}

void World::UpdateTerrainModifiersGPU_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    dxResources.UpdateBuffer(
        dxResources.terrainModBuffer_, 
        terrainModifiers_.GetData(), 
        sizeof(TerrainModifier) * terrainModifiers_.GetCount()
    );
}

#include "World.h"
#include "../../Resource/DXResources.h"
#include "../../Helpers/ChunkHelpers.h"

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
    DXMesh& chunkMesh = dxResources.worldMeshes_[chunk.x][chunk.y][chunk.z][backBuffer_];

    D3D11_MAPPED_SUBRESOURCE chunkDataResource;
    context->Map(dxResources.perChunkCBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &chunkDataResource);
    memcpy(chunkDataResource.pData, &chunkPos, sizeof(vec4)); 
    context->Unmap(dxResources.perChunkCBuffer_, 0);

    ID3D11ShaderResourceView* resources[2] = { dxResources.noiseTextureSRV_, dxResources.terrainModSRV_ };
    context->CSSetShaderResources(0, 2, resources);
    context->CSSetSamplers(0, 1, &dxResources.textureSampler_);
    UINT zeroes[4] = {0, 0, 0, 0};

    context->CSSetShader(dxResources.computeWVertsShader_, nullptr, 0);
    ID3D11UnorderedAccessView* vertView[3] = {
        dxResources.computeWVertsView_, 
        dxResources.computeWVoxelsView_, 
        dxResources.computeWCountView_
    };
    context->CSSetUnorderedAccessViews(0, 3, vertView, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.computeWVertsView_, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.computeWVoxelsView_, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.computeWCountView_, zeroes);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    context->CopyResource(chunkMesh.vertexBuffer, dxResources.computeWVertsBuffer_);

    context->CSSetShader(dxResources.computeWQuadsShader_, nullptr, 0);
    ID3D11UnorderedAccessView* quadView[4] = {
        dxResources.computeWVertsView_, 
        dxResources.computeWVoxelsView_, 
        dxResources.computeWQuadsView_, 
        dxResources.computeWCountView_
    };
    context->CSSetUnorderedAccessViews(0, 4, quadView, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.computeWQuadsView_, zeroes);
    context->ClearUnorderedAccessViewUint(dxResources.computeWCountView_, zeroes);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    context->CopyResource(chunkMesh.indexBuffer, dxResources.computeWQuadsBuffer_);

    context->CopyResource(dxResources.computeWCountOutput_, dxResources.computeWCountBuffer_);

    D3D11_MAPPED_SUBRESOURCE countResource;
    context->Map(dxResources.computeWCountOutput_, 0, D3D11_MAP_READ, 0, &countResource);
    chunkMesh.indexCount = reinterpret_cast<uint*>(countResource.pData)[0];
    context->Unmap(dxResources.computeWCountOutput_, 0);
}

void World::UpdateModifiersGPU_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    D3D11_MAPPED_SUBRESOURCE terrainModData;
    context->Map(dxResources.terrainModBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &terrainModData);
    memcpy(terrainModData.pData, terrainModifiers_.GetData(), sizeof(TerrainModifier) * MAX_TERRAIN_MODIFIERS);
    context->Unmap(dxResources.terrainModBuffer_, 0);
}

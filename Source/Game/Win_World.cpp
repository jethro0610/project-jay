#include "World.h"
#include "../Resource/DXResources.h"
#include "../Logging/Logger.h"

void World::GenerateNoiseTexture_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    D3D11_MAPPED_SUBRESOURCE noiseTextureResouce;
    context->Map(dxResources.noiseTexture_, 0, D3D11_MAP_WRITE_DISCARD, 0, &noiseTextureResouce);
    float* noiseArray = new float[NOISE_SIZE * NOISE_SIZE];

    for (int x = 0; x < NOISE_SIZE; x++)
    for (int y = 0; y < NOISE_SIZE; y++) {
        float xOffset = float(x) - NOISE_SIZE / 2.0f;
        float yOffset = float(y) - NOISE_SIZE / 2.0f;
        float noiseVal = noise_->GetNoise(xOffset, yOffset);
        *(noiseArray + y * NOISE_SIZE + x) = noiseVal;
    }

    memcpy(noiseTextureResouce.pData, noiseArray, NOISE_SIZE * NOISE_SIZE * sizeof(float));
    context->Unmap(dxResources.noiseTexture_, 0);
}

void World::GetMeshVerticesGPU_P(ivec3 chunk, std::vector<WorldVertex>& outVertices) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;
    vec4 chunkPos = vec4(vec3(chunk) * CHUNK_SIZE, 0.0f);

    D3D11_MAPPED_SUBRESOURCE chunkDataResource;
    context->Map(dxResources.perChunkCBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &chunkDataResource);
    memcpy(chunkDataResource.pData, &chunkPos, sizeof(vec4)); 
    context->Unmap(dxResources.perChunkCBuffer_, 0);

    context->CSSetShader(dxResources.computeWVertsShader_, nullptr, 0);
    ID3D11UnorderedAccessView* views[2] = {dxResources.computeWIMapView_, dxResources.computeWVertsViewA_};
    UINT counts[2] = {0, 0};
    context->CSSetUnorderedAccessViews(0, 2, views, counts);
    context->CSSetShaderResources(0, 1, &dxResources.noiseTextureSRV_);
    context->CSSetSamplers(0, 1, &dxResources.textureSampler_);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);

    context->CopyResource(dxResources.computeWVertsOutputA_, dxResources.computeWVertsBufferA_);
    D3D11_MAPPED_SUBRESOURCE computeVertexOutputResourceA;
    context->Map(dxResources.computeWVertsOutputA_, 0, D3D11_MAP_READ, 0, &computeVertexOutputResourceA);

    context->CopyResource(dxResources.computeWIMapOutput_, dxResources.computeWIMapBuffer_);
    D3D11_MAPPED_SUBRESOURCE computeIMapOutputResource;
    context->Map(dxResources.computeWIMapOutput_, 0, D3D11_MAP_READ, 0, &computeIMapOutputResource);
    memcpy(indexMap_, computeIMapOutputResource.pData, sizeof(int) * WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION);
    context->Unmap(dxResources.computeWIMapOutput_, 0);
    
    WorldVertex* vertices = reinterpret_cast<WorldVertex*>(computeVertexOutputResourceA.pData);

    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        int index = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);
        indicesDataChannel_[x][y][z] = outVertices.size();
        if (indexMap_[index] == -1)  {
            indicesDataChannel_[x][y][z] = -1;
            continue;
        }
        
        WorldVertex vertex = vertices[indexMap_[index]];
        outVertices.push_back(vertex);
    }
    context->Unmap(dxResources.computeWVertsOutputA_, 0);
}

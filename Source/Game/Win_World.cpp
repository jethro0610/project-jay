#include "World.h"
#include "../Resource/DXResources.h"
#include "../Logging/Logger.h"

void World::GenerateNoiseTexture_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    D3D11_MAPPED_SUBRESOURCE noiseTextureResouce;
    context->Map(dxResources.noiseTexture_, 0, D3D11_MAP_WRITE_DISCARD, 0, &noiseTextureResouce);
    float* noiseArray = new float[NOISE_SIZE * NOISE_SIZE];

    uint16_t noiseDetail = 1;
    uint16_t halfNoiseSize = NOISE_SIZE / noiseDetail / 2; 
    for (int x = 0; x < NOISE_SIZE; x++)
    for (int y = 0; y < NOISE_SIZE; y++) {
        float xOffset = float(x) / noiseDetail - halfNoiseSize;
        float yOffset = float(y) / noiseDetail - halfNoiseSize;
        float noiseVal = noise_->GetNoise(xOffset, yOffset);
        *(noiseArray + y * NOISE_SIZE + x) = noiseVal;
    }

    memcpy(noiseTextureResouce.pData, noiseArray, NOISE_SIZE * NOISE_SIZE * sizeof(float));
    context->Unmap(dxResources.noiseTexture_, 0);
}

void World::GenerateMeshGPU_P(ivec3 chunk) {
    ivec3 normalizedChunk = chunk;
    normalizedChunk.x += MAX_X_CHUNKS / 2;
    normalizedChunk.y += MAX_Y_CHUNKS / 2;
    normalizedChunk.z += MAX_Z_CHUNKS / 2;

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

    context->CopyResource(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].vertexBuffer, dxResources.computeWVertsBufferA_);

    // context->CSSetShader(dxResources.computeWTrisShader_, nullptr, 0);
    // ID3D11UnorderedAccessView* triViews[2] = {dxResources.computeWIMapView_, dxResources.computeWTrisView_};
    // context->CSSetUnorderedAccessViews(0, 2, triViews, counts);
    // context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    // context->CopyResource(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, dxResources.computeWTrisBuffer_);
    //
    // int tris[MAX_CHUNK_INDICES];
    // context->CopyResource(dxResources.computeWTrisOutput_, dxResources.computeWTrisBuffer_);
    // D3D11_MAPPED_SUBRESOURCE computeTrisOutputResource;
    // context->Map(dxResources.computeWTrisOutput_, 0, D3D11_MAP_READ, 0, &computeTrisOutputResource);
    // memcpy(tris, computeTrisOutputResource.pData, sizeof(int) * MAX_CHUNK_INDICES);
    // context->Unmap(dxResources.computeWTrisOutput_, 0);

    // context->CopyResource(dxResources.computeWIMapOutput_, dxResources.computeWIMapBuffer_);
    // D3D11_MAPPED_SUBRESOURCE computeIMapOutputResource;
    // context->Map(dxResources.computeWIMapOutput_, 0, D3D11_MAP_READ, 0, &computeIMapOutputResource);
    // memcpy(indexMap_, computeIMapOutputResource.pData, sizeof(int) * WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION);
    // context->Unmap(dxResources.computeWIMapOutput_, 0);
    //
    // for (int x = 0; x < WORLD_RESOLUTION; x++)
    // for (int y = 0; y < WORLD_RESOLUTION; y++)
    // for (int z = 0; z < WORLD_RESOLUTION; z++) {
    //     int index = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);
    //     if (indexMap_[index] == -1)  {
    //         continue;
    //     }
    // }
    // context->Unmap(dxResources.computeWVertsOutputA_, 0);
    //
    // std::vector<uint> indices;
    // indices.insert(indices.begin(), MAX_CHUNK_INDICES, -1);
    // GetMeshIndices(chunk, indices);
    //
    // D3D11_MAPPED_SUBRESOURCE indexResource;
    // context->Map(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource);
    // memcpy(indexResource.pData, indices.data(), sizeof(uint) * indices.size());
    // context->Unmap(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, 0);
    // dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexCount = indices.size();
}

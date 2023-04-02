#include "World.h"
#include "../Resource/DXResources.h"
#include "../Logging/Logger.h"

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

    context->CSSetShaderResources(0, 1, &dxResources.noiseTextureSRV_);
    context->CSSetSamplers(0, 1, &dxResources.textureSampler_);

    context->CSSetShader(dxResources.computeWVertsShader_, nullptr, 0);
    ID3D11UnorderedAccessView* vertView[2] = {dxResources.computeWVertsView_, dxResources.computeWValidView_};
    context->CSSetUnorderedAccessViews(0, 2, vertView, nullptr);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    context->CopyResource(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].vertexBuffer, dxResources.computeWVertsBuffer_);

    // context->CSSetShader(dxResources.computeWQuadsShader_, nullptr, 0);
    // ID3D11UnorderedAccessView* quadView[3] = {dxResources.computeWVertsView_, dxResources.computeWValidView_, dxResources.computeWQuadsView_};
    // context->CSSetUnorderedAccessViews(0, 3, quadView, nullptr);
    // context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);
    // context->CopyResource(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, dxResources.computeWQuadsBuffer_);
    // return;

    D3D11_MAPPED_SUBRESOURCE quadInfosResource;
    context->CopyResource(dxResources.computeWValidOutput_, dxResources.computeWValidBuffer_);
    context->Map(dxResources.computeWValidOutput_, 0, D3D11_MAP_READ, 0, &quadInfosResource);
    QuadInfo* quadInfos = reinterpret_cast<QuadInfo*>(quadInfosResource.pData);
    
    std::vector<uint> indices;
    indices.reserve(MAX_CHUNK_QUADS * 6);

    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        uint key = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);
        QuadInfo quadInfo = quadInfos[key];
        if (quadInfo.valid != 1)
            continue;

        for (int e = 0; e < 3; e++) {
            if (quadInfo.hasQuad[e] != 1)
                continue;
            int indiceCount = 0;
            uint planeIndices[4]; 
            for (int t = 0; t < 4; t++) {
                ivec3 indexPosition = ivec3(x, y, z) + triangulationTable[e][t];
                if (indexPosition.x < 0 ||
                    indexPosition.y < 0 ||
                    indexPosition.z < 0) {
                    break;
                }
                if (indexPosition.x >= WORLD_RESOLUTION ||
                    indexPosition.y >= WORLD_RESOLUTION ||
                    indexPosition.z >= WORLD_RESOLUTION) {
                    break;
                }

                uint otherKey = (indexPosition.z) + (indexPosition.y * WORLD_RESOLUTION) + (indexPosition.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
                if (quadInfos[otherKey].valid != 1)
                    break;

                planeIndices[t] = otherKey;
                indiceCount++;
            }

            if (indiceCount == 4) {
                if (quadInfo.forward[e] == 1) {
                    indices.push_back(planeIndices[0]);
                    indices.push_back(planeIndices[3]);
                    indices.push_back(planeIndices[2]);
                    indices.push_back(planeIndices[2]);
                    indices.push_back(planeIndices[1]);
                    indices.push_back(planeIndices[0]);
                }
                else {
                    indices.push_back(planeIndices[0]);
                    indices.push_back(planeIndices[1]);
                    indices.push_back(planeIndices[2]);
                    indices.push_back(planeIndices[2]);
                    indices.push_back(planeIndices[3]);
                    indices.push_back(planeIndices[0]);
                }
            }
        }
    }
    context->Unmap(dxResources.computeWValidOutput_, 0);

    D3D11_MAPPED_SUBRESOURCE indicesResource;
    context->Map(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indicesResource);
    memcpy(indicesResource.pData, indices.data(), indices.size() * sizeof(uint));
    context->Unmap(dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexBuffer, 0);
    dxResources.worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexCount = indices.size();
}

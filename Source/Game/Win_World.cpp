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
    ID3D11UnorderedAccessView* views[2] = {dxResources.computeWVertsView_, dxResources.computeWIMapView_};
    context->CSSetUnorderedAccessViews(0, 2, views, nullptr);
    context->CSSetShaderResources(0, 1, &dxResources.noiseTextureSRV_);
    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);

    context->CopyResource(dxResources.computeWVertsOutput_, dxResources.computeWVertsBuffer_);
    D3D11_MAPPED_SUBRESOURCE computeVertexOutputResource;
    context->Map(dxResources.computeWVertsOutput_, 0, D3D11_MAP_READ, 0, &computeVertexOutputResource);

    context->CopyResource(dxResources.computeWIMapOutput_, dxResources.computeWIMapBuffer_);
    D3D11_MAPPED_SUBRESOURCE computeIMapOutputResource;
    context->Map(dxResources.computeWIMapOutput_, 0, D3D11_MAP_READ, 0, &computeIMapOutputResource);
    memcpy(indicesDataChannel_, computeIMapOutputResource.pData, sizeof(int) * WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION);
    context->Unmap(dxResources.computeWIMapOutput_, 0);
    
    ComputeVertex* vertices = reinterpret_cast<ComputeVertex*>(computeVertexOutputResource.pData);
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        int index = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);
        if (vertices[index].valid == false) {
            indicesDataChannel_[x][y][z] = -1;
            continue;
        }

        WorldVertex worldVertex;
        worldVertex.position = vertices[index].position;
        worldVertex.normal = GetNormal(worldVertex.position, 2.0f);
        indicesDataChannel_[x][y][z] = outVertices.size();
        outVertices.push_back(worldVertex);
    }
    context->Unmap(dxResources.computeWVertsOutput_, 0);

}

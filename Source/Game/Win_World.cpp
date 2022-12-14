#include "World.h"
#include "../Resource/DXResources.h"

void World::GetMeshVerticesGPU_P(void* graphicsResources, ivec3 chunk, std::vector<WorldVertex>& outVertices) {
    DXResources* dxResources = static_cast<DXResources*>(graphicsResources);
    ID3D11DeviceContext* context = dxResources->context_;

    D3D11_MAPPED_SUBRESOURCE distanceCacheResource;
    context->Map(dxResources->distanceCacheBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &distanceCacheResource);
    memcpy(distanceCacheResource.pData, localDistanceCache_, sizeof(float) * DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE);
    context->Unmap(dxResources->distanceCacheBuffer_, 0);

    context->CSSetShader(dxResources->computeWVertsShader_, nullptr, 0);
    context->CSSetShaderResources(0, 1, &dxResources->distanceCacheView_);
    context->CSSetUnorderedAccessViews(0, 1, &dxResources->computeWVertsView_, nullptr);

    context->Dispatch(WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS, WORLD_COMPUTE_GROUPS);

    context->CopyResource(dxResources->computeWVertsOutput_, dxResources->computeWVertsBuffer_);
    D3D11_MAPPED_SUBRESOURCE computeVertexOutputResource;
    context->Map(dxResources->computeWVertsOutput_, 0, D3D11_MAP_READ, 0, &computeVertexOutputResource);
    
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    vec3* vertices = reinterpret_cast<vec3*>(computeVertexOutputResource.pData);
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        int index = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);
        if (vertices[index].x == -1.0f) {
            continue;
        }

        WorldVertex worldVertex;
        worldVertex.position = vertices[index] + chunkOffset;
        worldVertex.normal = GetNormal(worldVertex.position, 2.0f);
        indicesDataChannel_[x][y][z] = outVertices.size();
        outVertices.push_back(worldVertex);
    }
    context->Unmap(dxResources->computeWVertsOutput_, 0);
}

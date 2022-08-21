#include "World.h"
#include "../Resource/DXResources.h"

#include "../Logging/Logger.h"

void World::GetMeshVerticesGPU_P(void* graphicsResources, ivec3 coordinates, std::vector<WorldVertex>& outVertices) {
    DXResources* dxResources = static_cast<DXResources*>(graphicsResources);
    ID3D11DeviceContext* context = dxResources->context_;

    D3D11_MAPPED_SUBRESOURCE distanceCacheResource;
    context->Map(dxResources->distanceCacheBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &distanceCacheResource);
    memcpy(distanceCacheResource.pData, localDistanceCache_, sizeof(float) * DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE);
    context->Unmap(dxResources->distanceCacheBuffer_, 0);

    context->CSSetShader(dxResources->computeVertexShader_, nullptr, 0);
    context->CSSetShaderResources(0, 1, &dxResources->distanceCacheView_);
    context->CSSetUnorderedAccessViews(0, 1, &dxResources->computeVertexView_, nullptr);

    context->Dispatch(1, 1, 1);

    context->CopyResource(dxResources->computeVertexOutput_, dxResources->computeVertexBuffer_);
    D3D11_MAPPED_SUBRESOURCE computeVertexOutputResource;
    context->Map(dxResources->computeVertexOutput_, 0, D3D11_MAP_READ, 0, &computeVertexOutputResource);
    
    vec3* vertices = reinterpret_cast<vec3*>(computeVertexOutputResource.pData);
    for (int x = 0; x < COMPUTE_VERTICES_GROUP_SIZE; x++)
    for (int y = 0; y < COMPUTE_VERTICES_GROUP_SIZE; y++)
    for (int z = 0; z < COMPUTE_VERTICES_GROUP_SIZE; z++) {
        int index = (z) + (y * COMPUTE_VERTICES_GROUP_SIZE) + (x * COMPUTE_VERTICES_GROUP_SIZE * COMPUTE_VERTICES_GROUP_SIZE);
        if (vertices[index].x == -1.0f) {
            indicesDataChannel_[x][y][z] = -1;
            continue;
        }

        WorldVertex worldVertex;
        worldVertex.position = vertices[index];
        worldVertex.normal = vec3(0.0f, 1.0f, 0.0f);
        indicesDataChannel_[x][y][z] = outVertices.size();
        outVertices.push_back(worldVertex);
    }
    context->Unmap(dxResources->computeVertexOutput_, 0);
}
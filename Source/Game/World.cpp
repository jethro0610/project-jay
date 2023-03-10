#include "World.h"
#include <cmath>
#include "../Logging/Logger.h"
#include <gtx/string_cast.hpp>
using namespace glm;

World::World(Entity* entities, TerrainModComponent& terrainModComponent):
    noise_(FastNoiseLite()),
    entities_(entities),
    terrainModComponent_(terrainModComponent)
{

}

void World::FillLocalDistanceCache(ivec3 chunk) {
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    for (int x = 0; x < DISTANCE_CACHE_SIZE; x++)
    for (int y = 0; y < DISTANCE_CACHE_SIZE; y++)
    for (int z = 0; z < DISTANCE_CACHE_SIZE; z++) {
        vec3 voxelOffset = vec3(x, y, z) * VOXEL_SIZE;
        localDistanceCache_[x][y][z] = GetDistance(chunkOffset + voxelOffset);
    }
}

float World::GetDistance(vec3 position) const {
    // Sample perlin noise in a circle following the position to form a blob
    vec2 noiseDir = normalize(vec2(position.x, position.z)) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = noise_.GetNoise<float>(noiseDir.x, noiseDir.y) * 32.0f;

    float radius = 160.0f + blobRadius;

    float noiseHeight = noise_.GetNoise<float>(position.x * 0.75f, position.z * 0.75f) * 8.0f + 8.0f;
    float height = 32.0f + noiseHeight;

    vec2 d = vec2(length(vec2(position.x, position.z)), abs(position.y)) - vec2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    float modDist = INFINITY;
    // TODO: We check entities on every distance
    // It's more efficient to precauclate the list of modifying entities
    // Implement this later
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities_[i].alive_)
            continue;
        if (!entities_[i].HasComponent(terrainModComponent_))
            continue;

        float dist = distance(terrainModComponent_.position[i], position) - terrainModComponent_.radius[i]; 
        modDist = min(dist, modDist);
    }

    return min(blobDist, modDist);
}

vec3 World::GetNormal(vec3 position, float epsilon) const {
    vec3 dX = position + vec3(epsilon, 0.0f, 0.0f);
    vec3 dY = position + vec3(0.0f, epsilon, 0.0f);
    vec3 dZ = position + vec3(0.0f, 0.0f, epsilon);

    float gradX = GetDistance(dX);
    float gradY = GetDistance(dY);
    float gradZ = GetDistance(dZ);
    return normalize(vec3(gradX, gradY, gradZ));
}

void World::GetMesh(ivec3 chunk, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices) {
    FillLocalDistanceCache(chunk);
    GetMeshVerticesCPU(chunk, outVertices);
    GetMeshIndices(chunk, outIndices);

    assert(outVertices.size() <= MAX_CHUNK_VERTICES);
    assert(outIndices.size() <= MAX_CHUNK_INDICES);
}

void World::GetMeshGPUCompute(void* graphicsResources, ivec3 chunk, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices) {
    FillLocalDistanceCache(chunk);
    GetMeshVerticesGPU_P(graphicsResources, chunk, outVertices);
    GetMeshIndices(chunk, outIndices);

    assert(outVertices.size() <= MAX_CHUNK_VERTICES);
    assert(outIndices.size() <= MAX_CHUNK_INDICES);
}

void World::GetMeshVerticesCPU(ivec3 chunk, std::vector<WorldVertex>& outVertices) {
    uint16_t currentIndex = 0;
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        ivec3 localVoxelPosition(x, y, z);
        vec3 sumOfIntersections(0.0f, 0.0f, 0.0f);
        int totalIntersections = 0;

        for (int e = 0; e < 12; e++) {
            ivec3 localEdgeStart = localVoxelPosition + cornerTable[edgeTable[e][0]];
            ivec3 localEdgeEnd = localVoxelPosition + cornerTable[edgeTable[e][1]];
            float edgeStartDistance = localDistanceCache_[localEdgeStart.x][localEdgeStart.y][localEdgeStart.z];
            float edgeEndDistance = localDistanceCache_[localEdgeEnd.x][localEdgeEnd.y][localEdgeEnd.z];

            // If the value is negative, it implies the two signs are different, 
            // so there must be an intersection on this edge
            if (edgeStartDistance * edgeEndDistance <= 0.0f) {
                float differenceRatio = (float)edgeStartDistance / (edgeStartDistance - edgeEndDistance);
                vec3 intersection(0.0f, 0.0f, 0.0f);

                if (edgeStartDistance - edgeEndDistance == 0.0f)
                    intersection = (vec3(localEdgeStart) + vec3(localEdgeEnd)) / 2.0f;
                else
                    intersection = (1.0f - differenceRatio) * vec3(localEdgeStart) + differenceRatio * vec3(localEdgeEnd);

                sumOfIntersections += intersection;
                totalIntersections++;
            }
        }
        if (totalIntersections == 0) {
            indicesDataChannel_[x][y][z] = -1;
        }
        else {
            indicesDataChannel_[x][y][z] = currentIndex;
            WorldVertex vertex;
            vertex.position = sumOfIntersections / (float)totalIntersections; // voxelPosition gives cube vertices
            vertex.position *= VOXEL_SIZE;
            vertex.position += chunkOffset;
            vertex.normal = GetNormal(vertex.position, 2.0f);
            outVertices.push_back(vertex);
            currentIndex++;
        }
    }
}

void World::GetMeshIndices(ivec3 chunk, std::vector<uint16_t>& outIndices) {
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        ivec3 localVoxelPosition = vec3(x, y, z);

        float edgeStartDistance = localDistanceCache_[localVoxelPosition.x][localVoxelPosition.y][localVoxelPosition.z];
        if (edgeStartDistance == 0.0f)
            edgeStartDistance = 1.0f;

        for (int e = 0; e < 3; e++) {
            ivec3 localEdgeEnd = localVoxelPosition + triangleEdgeTable[e];
            float edgeEndDistance = localDistanceCache_[localEdgeEnd.x][localEdgeEnd.y][localEdgeEnd.z];
            if (edgeEndDistance == 0.0f)
                edgeEndDistance = 1.0f;

            if (edgeStartDistance * edgeEndDistance <= 0) {
                int indiceCount = 0;
                int planeIndices[4] = { -1, -1, -1, -1 };
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

                    int index = indicesDataChannel_[indexPosition.x][indexPosition.y][indexPosition.z];
                    if (index == -1) {
                        break;
                    }

                    planeIndices[t] = index;
                    indiceCount++;
                }

                if (indiceCount == 4) {
                    if (edgeStartDistance > edgeEndDistance) {
                        outIndices.push_back(planeIndices[0]);
                        outIndices.push_back(planeIndices[3]);
                        outIndices.push_back(planeIndices[2]);
                        outIndices.push_back(planeIndices[2]);
                        outIndices.push_back(planeIndices[1]);
                        outIndices.push_back(planeIndices[0]);
                    }
                    else {
                        outIndices.push_back(planeIndices[0]);
                        outIndices.push_back(planeIndices[1]);
                        outIndices.push_back(planeIndices[2]);
                        outIndices.push_back(planeIndices[2]);
                        outIndices.push_back(planeIndices[3]);
                        outIndices.push_back(planeIndices[0]);
                    }
                }
            }
        }
    }
}

float World::Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void World::MarkChunkDirty(ivec3 chunk) {
    int index = (chunk.x) + (chunk.y * MAX_X_CHUNKS) + (chunk.z * MAX_X_CHUNKS * MAX_Y_CHUNKS); 
    dirtyChunks_[index] = true;
}

bool World::ChunkIsDirty(ivec3 chunk) const {
    int index = (chunk.x) + (chunk.y * MAX_X_CHUNKS) + (chunk.z * MAX_Y_CHUNKS * MAX_Z_CHUNKS); 
    return dirtyChunks_[index];
}

vec3 World::GetNearestInDirection(vec3 start, vec3 direction, uint16_t maxSteps) {
    vec3 currentPosition = start;
    for (int i = 0; i < maxSteps; i++) {
        float distance = GetDistance(currentPosition);
        if (abs(distance) < 0.001f)
            break;
        currentPosition += direction * distance;
    }
    return currentPosition;
}

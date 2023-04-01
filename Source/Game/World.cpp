#include "World.h"
#include <cmath>
#include "../Logging/Logger.h"
#include <gtx/string_cast.hpp>
using namespace glm;

World::World(Entity* entities, ResourceManager& resourceManager, TerrainModComponent& terrainModComponent):
    noise_(new FastNoiseLite()),
    entities_(entities),
    resourceManager_(resourceManager),
    terrainModComponent_(terrainModComponent)
{
    GenerateNoiseTexture_P();
}

float World::GetDistance(vec3 position) const {
    // Sample perlin noise in a circle following the position to form a blob
    vec2 position2d = vec2(position.x, position.z);
    float n = noise_->GetNoise(position2d.x, position2d.y);
    return position.y - n * 4.0f;

    vec2 noiseDir = normalize(position2d) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = noise_->GetNoise(noiseDir.x, noiseDir.y) * 32.0f;

    float radius = 160.0f + blobRadius;

    float noiseHeight = noise_->GetNoise(position2d.x * 0.75f, position2d.y * 0.75f) * 8.0f + 8.0f;
    float height = 32.0f + noiseHeight;

    vec2 d = vec2(length(position2d), abs(position.y)) - vec2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    return blobDist;
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
    GetMeshVerticesCPU(chunk, outVertices);
    // GetMeshIndices(chunk, outIndices);
}

void World::GetMeshVerticesCPU(ivec3 chunk, std::vector<WorldVertex>& outVertices) {
    uint16_t currentIndex = 0;
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        vec3 voxelPosition = vec3(x, y, z) * VOXEL_SIZE + chunkOffset;
        vec3 sumOfIntersections(0.0f, 0.0f, 0.0f);
        int totalIntersections = 0;

        for (int e = 0; e < 12; e++) {
            vec3 edgeStart = voxelPosition + cornerTable[edgeTable[e][0]] * VOXEL_SIZE;
            vec3 edgeEnd = voxelPosition + cornerTable[edgeTable[e][1]] * VOXEL_SIZE;
            float edgeStartDistance = GetDistance(edgeStart);
            float edgeEndDistance = GetDistance(edgeEnd);

            // If the value is negative, it implies the two signs are different, 
            // so there must be an intersection on this edge
            if (edgeStartDistance * edgeEndDistance <= 0.0f) {
                float differenceRatio = (float)edgeStartDistance / (edgeStartDistance - edgeEndDistance);
                vec3 intersection(0.0f, 0.0f, 0.0f);

                if (edgeStartDistance - edgeEndDistance == 0.0f)
                    intersection = (edgeStart + edgeEnd) / 2.0f;
                else
                    intersection = (1.0f - differenceRatio) * edgeStart + differenceRatio * edgeEnd;

                sumOfIntersections += intersection;
                totalIntersections++;
            }
        }
        int voxelId = (z) + (y * WORLD_RESOLUTION) + (x * WORLD_RESOLUTION * WORLD_RESOLUTION);

        if (totalIntersections == 0) {
            indexMap_[voxelId] = -1;
        }
        else {
            indexMap_[voxelId] = currentIndex;
            WorldVertex vertex;
            vertex.position = sumOfIntersections / (float)totalIntersections; // voxelPosition gives cube vertices
            // vertex.position *= VOXEL_SIZE;
            // vertex.position += chunkOffset;
            vertex.normal = GetNormal(vertex.position, 2.0f);
            outVertices.push_back(vertex);
            currentIndex++;
        }
    }
}

void World::GetMeshIndices(ivec3 chunk, std::vector<uint>& outIndices) {
    vec3 chunkOffset = vec3(chunk) * CHUNK_SIZE;
    int count = 0;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        vec3 voxelPosition = vec3(x, y, z) * VOXEL_SIZE + chunkOffset;
        float edgeStartDistance = GetDistance(voxelPosition);

        for (int e = 0; e < 3; e++) {
            vec3 edgeEnd = voxelPosition + triangleEdgeTable[e] * VOXEL_SIZE;
            float edgeEndDistance = GetDistance(edgeEnd);

            if (edgeStartDistance * edgeEndDistance <= 0.0f) {
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

                    int voxelId = (indexPosition.z) + (indexPosition.y * WORLD_RESOLUTION) + (indexPosition.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
                    int index = indexMap_[voxelId];
                    if (index == -1) {
                        break;
                    }

                    planeIndices[t] = index;
                    indiceCount++;
                }

                if (indiceCount == 4) {
                    int quadStart = count * 6;
                    if (edgeStartDistance > edgeEndDistance) {
                        outIndices[quadStart + 0] = planeIndices[0];
                        outIndices[quadStart + 1] = planeIndices[3];
                        outIndices[quadStart + 2] = planeIndices[2];
                        outIndices[quadStart + 3] = planeIndices[2];
                        outIndices[quadStart + 4] = planeIndices[1];
                        outIndices[quadStart + 5] = planeIndices[0];
                    }
                    else {
                        outIndices[quadStart + 0] = planeIndices[0];
                        outIndices[quadStart + 1] = planeIndices[1];
                        outIndices[quadStart + 2] = planeIndices[2];
                        outIndices[quadStart + 3] = planeIndices[2];
                        outIndices[quadStart + 4] = planeIndices[3];
                        outIndices[quadStart + 5] = planeIndices[0];
                    }
                    count++;
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

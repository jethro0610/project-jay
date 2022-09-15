#include "World.h"
#include <cmath>
using namespace glm;

World::World() {
    noise_ = new FastNoiseLite();
    InitHills();
}

void World::FillLocalDistanceCache(ivec3 coordinates) {
    vec3 coordinateOffset = vec3(coordinates) * COORDINATE_SIZE;
    for (int x = 0; x < DISTANCE_CACHE_SIZE; x++)
    for (int y = 0; y < DISTANCE_CACHE_SIZE; y++)
    for (int z = 0; z < DISTANCE_CACHE_SIZE; z++) {
        vec3 voxelOffset = vec3(x, y, z) * VOXEL_SIZE;
        localDistanceCache_[x][y][z] = GetDistance(coordinateOffset + voxelOffset);
    }
}

void World::InitHills() {
    hills_[0] = vec3(0.0f, 0.0f, 0.0f);
}

float World::GetDistance(vec3 position) const {
    // Sample perlin noise in a circle following the position to form a blob
    vec2 noiseDir = normalize(vec2(position.x, position.z)) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = noise_->GetNoise<float>(noiseDir.x, noiseDir.y) * 32.0f;

    float radius = 160.0f + blobRadius;

    float noiseHeight = noise_->GetNoise<float>(position.x * 0.75f, position.z * 0.75f) * 8.0f + 8.0f;
    float height = 32.0f + noiseHeight;

    vec2 d = vec2(length(vec2(position.x, position.z)), abs(position.y)) - vec2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    float hillDist = INFINITY;
    for (int i = 0; i < 1; i++) {
        float curHillDist = distance(hills_[i], position) - 64.0f; 
        hillDist = curHillDist;
    }
    return min(hillDist, blobDist);
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

void World::GetMesh(ivec3 coordinates, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices) {
    FillLocalDistanceCache(coordinates);
    GetMeshVerticesCPU(coordinates, outVertices);
    GetMeshIndices(coordinates, outIndices);

    assert(outVertices.size() <= MAX_COORDINATE_VERTICES);
    assert(outIndices.size() <= MAX_COORDINATE_INDICES);
}

void World::GetMeshGPUCompute(void* graphicsResources, ivec3 coordinates, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices) {
    FillLocalDistanceCache(coordinates);
    GetMeshVerticesGPU_P(graphicsResources, coordinates, outVertices);
    GetMeshIndices(coordinates, outIndices);

    assert(outVertices.size() <= MAX_COORDINATE_VERTICES);
    assert(outIndices.size() <= MAX_COORDINATE_INDICES);
}

void World::GetMeshVerticesCPU(ivec3 coordinates, std::vector<WorldVertex>& outVertices) {
    uint16_t currentIndex = 0;
    vec3 coordinateOffset = vec3(coordinates) * COORDINATE_SIZE;
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
            vertex.position += coordinateOffset;
            vertex.normal = GetNormal(vertex.position, 2.0f);
            outVertices.push_back(vertex);
            currentIndex++;
        }
    }
}

void World::GetMeshIndices(ivec3 coordinates, std::vector<uint16_t>& outIndices) {
    vec3 coordinateOffset = vec3(coordinates) * COORDINATE_SIZE;
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

#include "World.h"
#include <FastNoiseLite.h>
#include "../Types/Transform.h"

#include "../Logging/Logger.h"

float World::GetDistance(vec3 position) const {
    FastNoiseLite fastNoise;
    float height = fastNoise.GetNoise<float>(position.x * 5.0f, position.z * 5.0f) * 3.0f + 8.0f;
    return position.y - height;
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

void World::Temp_Generate(std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices) {
    Temp_GenerateVertices(outVertices);
    Temp_GenerateIndices(outIndices);
}

void World::Temp_GenerateVertices (std::vector<WorldVertex>& outVertices) {
    uint16_t currentIndex = 0;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        vec3 voxelPosition(x, y, z);
        vec3 sumOfIntersections(0.0f, 0.0f, 0.0f);
        int totalIntersections = 0;

        for (int e = 0; e < 12; e++) {
            vec3 edgeStartPosition = voxelPosition + vec3(cornerTable[edgeTable[e][0]]);
            vec3 edgeEndPosition = voxelPosition + vec3(cornerTable[edgeTable[e][1]]);
            float edgeStartDistance = GetDistance(edgeStartPosition);
            float edgeEndDistance = GetDistance(edgeEndPosition);

            // If the value is negative, it implies the two signs are different, 
            // so there must be an intersection on this edge
            if (edgeStartDistance * edgeEndDistance <= 0.0f) {
                float differenceRatio = (float)edgeStartDistance / (edgeStartDistance - edgeEndDistance);
                vec3 intersection(0.0f, 0.0f, 0.0f);

                if (edgeStartDistance - edgeEndDistance == 0.0f)
                    intersection = (vec3(edgeStartPosition) + vec3(edgeEndPosition)) / 2.0f;
                else
                    intersection = (1.0f - differenceRatio) * vec3(edgeStartPosition) + differenceRatio * vec3(edgeEndPosition);

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
            vertex.normal = GetNormal(vertex.position, 2.0f);
            outVertices.push_back(vertex);
            currentIndex++;
        }
    }
}

void World::Temp_GenerateIndices(std::vector<uint16_t>& outIndices) {
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        vec3 voxelPosition(x, y, z);
        float edgeStartDistance = GetDistance(voxelPosition);
        if (edgeStartDistance == 0.0f)
            edgeStartDistance = 1.0f;

        for (int e = 0; e < 3; e++) {
            vec3 edgeEndPosition = voxelPosition + vec3(triangleEdgeTable[e]);
            float edgeEndDistance = GetDistance(edgeEndPosition);
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
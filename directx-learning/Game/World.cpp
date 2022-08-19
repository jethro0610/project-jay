#include "World.h"
#include <FastNoiseLite.h>
#include "../Types/Transform.h"

#include "../Logging/Logger.h"

void World::Temp_FillDistanceField() {
    FastNoiseLite fastNoise;

    for (int x = 0; x < DISTANCE_FIELD_SIZE; x++)
    for (int y = 0; y < DISTANCE_FIELD_SIZE; y++)
    for (int z = 0; z < DISTANCE_FIELD_SIZE; z++) {
        vec3 position(x, y, z);
        float height = fastNoise.GetNoise<float>(x * 5.0f, z * 5.0f) * 3.0f + 8.0f;
        distanceField_[x][y][z] = y - height;
    }
}

void World::Temp_Generate(std::vector<vec3>& outVertices, std::vector<uint16_t>& outIndices) {
    Temp_GenerateVertices(outVertices);
    Temp_GenerateIndices(outIndices);
}

void World::Temp_GenerateVertices (std::vector<vec3>& outVertices) {
    uint16_t currentIndex = 0;
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        ivec3 voxelPosition(x, y, z);
        vec3 sumOfIntersections(0.0f, 0.0f, 0.0f);
        int totalIntersections = 0;

        for (int e = 0; e < 12; e++) {
            ivec3 edgeStartPosition = voxelPosition + cornerTable[edgeTable[e][0]];
            ivec3 edgeEndPosition = voxelPosition + cornerTable[edgeTable[e][1]];
            float edgeStartDistance = distanceField_[edgeStartPosition.x][edgeStartPosition.y][edgeStartPosition.z];
            float edgeEndDistance = distanceField_[edgeEndPosition.x][edgeEndPosition.y][edgeEndPosition.z];

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
            indicesDataBuffer_[x][y][z] = -1;
        }
        else {
            indicesDataBuffer_[x][y][z] = currentIndex;
            outVertices.push_back(sumOfIntersections / (float)totalIntersections);
            //outVertices.push_back(voxelPosition); This create unsmoothed, cubic vertices. May want to experiment with it later
            currentIndex++;
        }
    }
}

void World::Temp_GenerateIndices(std::vector<uint16_t>& outIndices) {
    for (int x = 0; x < WORLD_RESOLUTION; x++)
    for (int y = 0; y < WORLD_RESOLUTION; y++)
    for (int z = 0; z < WORLD_RESOLUTION; z++) {
        ivec3 voxelPosition(x, y, z);
        float edgeStartDistance = distanceField_[x][y][z];
        if (edgeStartDistance == 0.0f)
            edgeStartDistance = 1.0f;

        for (int e = 0; e < 3; e++) {
            ivec3 edgeEndPosition = voxelPosition + triangleEdgeTable[e];
            float edgeEndDistance = distanceField_[edgeEndPosition.x][edgeEndPosition.y][edgeEndPosition.z];
            if (edgeEndDistance == 0.0f)
                edgeEndDistance = 1.0f;

            if (edgeStartDistance * edgeEndDistance <= 0) {
                int indiceCount = 0;
                int planeIndices[4] = { -1, -1, -1, -1 };
                for (int t = 0; t < 4; t++) {
                    ivec3 indexPosition = voxelPosition + triangulationTable[e][t];

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

                    int index = indicesDataBuffer_[indexPosition.x][indexPosition.y][indexPosition.z];
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
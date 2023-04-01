#include "WorldFunctions.hlsli"
Texture2D noiseTex : register(t0);
SamplerState noiseSamp;

cbuffer perChunkData : register(b0) {
    float4 chunkPos;
}

static float3 triangleEdgeTable[3] = {
    {1.0f, 0.0f, 0.0f},  // X
    {0.0f, 1.0f, 0.0f},  // Y
    {0.0f, 0.0f, 1.0f},  // Z
};

static int3 triangulationTable[3][4] = {
    {{ 0,  0,  0}, { 0,  0, -1}, { 0, -1, -1}, { 0, -1,  0}},   // X; Why is this LHS and backwards tf.
    {{ 0,  0,  0}, {-1,  0,  0}, {-1,  0, -1}, { 0,  0, -1}},   // Y
    {{ 0,  0,  0}, { 0, -1,  0}, {-1, -1,  0}, {-1,  0,  0}},   // Z
};

static uint WORLD_RESOLUTION = 16;
static uint WORLD_COMPUTE_GROUPS = WORLD_RESOLUTION / 8;
static uint DISTANCE_CACHE_SIZE = WORLD_RESOLUTION + 1;
static float CHUNK_SIZE = 32.0f;
static float VOXEL_SIZE = CHUNK_SIZE / (WORLD_RESOLUTION - 1);
static float GROUP_OFFSET = WORLD_RESOLUTION / WORLD_COMPUTE_GROUPS;

RWStructuredBuffer<int> indexMap : register(u0);
RWStructuredBuffer<uint> tris : register(u1);

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelIndex = groupId * GROUP_OFFSET + threadId;

    float3 voxelPosition = float3(localVoxelIndex) * VOXEL_SIZE + chunkPos;
    float edgeStartDistance = GetDistance(voxelPosition, noiseTex, noiseSamp);

    for (int e = 0; e < 3; e++) {
        float3 edgeEnd = voxelPosition + triangleEdgeTable[e] * VOXEL_SIZE;
        float edgeEndDistance = GetDistance(edgeEnd, noiseTex, noiseSamp);

        if (edgeStartDistance * edgeEndDistance <= 0.0f) {
            int indiceCount = 0;
            int planeIndices[4] = { -1, -1, -1, -1 };
            for (int t = 0; t < 4; t++) {
                int3 indexPosition = localVoxelIndex + triangulationTable[e][t];

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
                int index = indexMap[voxelId];
                if (index == -1) {
                    break;
                }

                planeIndices[t] = index;
                indiceCount++;
            }

            if (indiceCount == 4) {
                uint quadStart = (tris.IncrementCounter() - 1) * 6;
                if (edgeStartDistance > edgeEndDistance) {
                    tris[quadStart + 0] = planeIndices[0];
                    tris[quadStart + 1] = planeIndices[3];
                    tris[quadStart + 2] = planeIndices[2];
                    tris[quadStart + 3] = planeIndices[2];
                    tris[quadStart + 4] = planeIndices[1];
                    tris[quadStart + 5] = planeIndices[0];
                }
                else {
                    tris[quadStart + 0] = planeIndices[0];
                    tris[quadStart + 1] = planeIndices[1];
                    tris[quadStart + 2] = planeIndices[2];
                    tris[quadStart + 3] = planeIndices[2];
                    tris[quadStart + 4] = planeIndices[3];
                    tris[quadStart + 5] = planeIndices[0];
                }
            }
        }
    }
}

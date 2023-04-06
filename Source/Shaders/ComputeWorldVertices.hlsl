#include "WorldFunctions.hlsli"

cbuffer perChunkData : register(b0) {
    float4 chunkPos;
}

static float3 cornerTable[8] = {
    { 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f },
};

static int2 edgeTable[12] = {
    { 0, 1 },
    { 3, 2 },
    { 4, 5 },
    { 7, 6 },
    { 0, 3 },
    { 1, 2 },
    { 4, 7 },
    { 5, 6 },
    { 0, 4 },
    { 3, 7 },
    { 1, 5 },
    { 2, 6 },
};

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

struct WorldVertex {
    float3 pos;
    float3 norm;
};

struct VoxelInfo {
    bool valid;
    bool hasQuad[3];
    bool forward[3];
};

RWStructuredBuffer<WorldVertex> vertices : register(u0);
RWStructuredBuffer<VoxelInfo> voxelInfos : register(u1);
RWStructuredBuffer<uint> count : register(u2);

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelIndex = groupId * GROUP_OFFSET + threadId;
    uint key = (localVoxelIndex.z) + (localVoxelIndex.y * WORLD_RESOLUTION) + (localVoxelIndex.x * WORLD_RESOLUTION * WORLD_RESOLUTION);

    float3 voxelPosition = float3(localVoxelIndex) * VOXEL_SIZE + chunkPos;
    float3 sumOfIntersections = float3(0.0f, 0.0f, 0.0f);
    int totalIntersections = 0;

    for (int e = 0; e < 12; e++) {
        float3 edgeStart = voxelPosition + cornerTable[edgeTable[e][0]] * VOXEL_SIZE;
        float3 edgeEnd = voxelPosition + cornerTable[edgeTable[e][1]] * VOXEL_SIZE;
        
        float edgeStartDistance = GetDistance(edgeStart);
        float edgeEndDistance = GetDistance(edgeEnd);
        
        // If the value is negative, it implies the two signs are different, 
        // so there must be an intersection on this edge
        if (edgeStartDistance * edgeEndDistance <= 0.0f) {
            float differenceRatio = edgeStartDistance / (edgeStartDistance - edgeEndDistance);
            float3 intersection = float3(0.0f, 0.0f, 0.0f);

            if (edgeStartDistance - edgeEndDistance == 0.0f)
                intersection = (edgeStart + edgeEnd) / 2.0f;
            else
                intersection = (1.0f - differenceRatio) * edgeStart + differenceRatio * edgeEnd;

            sumOfIntersections += intersection;
            totalIntersections++;
        }
    }
    if (totalIntersections <= 0){
        voxelInfos[key].valid = false; 
    }
    else {
        InterlockedAdd(count[0], 1);
        vertices[key].pos = sumOfIntersections / (float)totalIntersections;
        vertices[key].norm = GetNormal(vertices[key].pos, 2.0f);
        voxelInfos[key].valid = true; 
    }

    float edgeStartDistance = GetDistance(voxelPosition);
    for (int e = 0; e < 3; e++) {
        float3 edgeEnd = voxelPosition + triangleEdgeTable[e] * VOXEL_SIZE;
        float edgeEndDistance = GetDistance(edgeEnd);

        if (edgeStartDistance * edgeEndDistance <= 0.0f)
            voxelInfos[key].hasQuad[e] = true; 
        else
            voxelInfos[key].hasQuad[e] = false; 

        if (edgeStartDistance > edgeEndDistance)
            voxelInfos[key].forward[e] = true;
        else
            voxelInfos[key].forward[e] = false;
    }

}

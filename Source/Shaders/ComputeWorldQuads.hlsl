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

struct WorldVertex {
    float3 pos;
    float3 norm;
};

struct QuadInfo {
    int valid;
    int hasQuad[3];
    int forward[4];
};

struct Quad {
    uint i0;
    uint i1;
    uint i2;
    uint i3;
    uint i4;
    uint i5;
};

RWStructuredBuffer<WorldVertex> vertices : register(u0);
RWStructuredBuffer<QuadInfo> quadInfos : register(u1);
AppendStructuredBuffer<Quad> indices : register(u2);
RWStructuredBuffer<uint> count : register(u3);

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelIndex = groupId * GROUP_OFFSET + threadId;
    float3 voxelPosition = float3(localVoxelIndex) * VOXEL_SIZE + chunkPos;
    float edgeStartDistance = GetDistance(voxelPosition, noiseTex, noiseSamp);
    uint key = (localVoxelIndex.z) + (localVoxelIndex.y * WORLD_RESOLUTION) + (localVoxelIndex.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
    QuadInfo quadInfo = quadInfos[key];
    if (quadInfo.valid != 1)
        return;

    for (int e = 0; e < 3; e++) {
        if (quadInfo.hasQuad[e] != 1)
            continue;
        int indiceCount = 0;
        uint planeIndices[4]; 
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

            uint otherKey = (indexPosition.z) + (indexPosition.y * WORLD_RESOLUTION) + (indexPosition.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
            if (quadInfos[otherKey].valid != 1)
                break;

            planeIndices[t] = otherKey;
            indiceCount++;
        }

        if (indiceCount == 4) {
            Quad newQuad;
            if (quadInfo.forward[e] == 1) {
                newQuad.i0 = planeIndices[0];
                newQuad.i1 = planeIndices[3];
                newQuad.i2 = planeIndices[2];
                newQuad.i3 = planeIndices[2];
                newQuad.i4 = planeIndices[1];
                newQuad.i5 = planeIndices[0];
            }
            else {
                newQuad.i0 = planeIndices[0];
                newQuad.i1 = planeIndices[1];
                newQuad.i2 = planeIndices[2];
                newQuad.i3 = planeIndices[2];
                newQuad.i4 = planeIndices[3];
                newQuad.i5 = planeIndices[0];
            }
            InterlockedAdd(count[0], 6);
            indices.Append(newQuad);
        }
    }
}

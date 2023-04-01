#include "WorldFunctions.hlsli"
Texture2D noiseTex : register(t0);
SamplerState noiseSamp;

cbuffer perChunkData : register(b0) {
    float4 chunkPos;
}

const float3 triangleEdgeTable[3] = {
    {1.0f, 0.0f, 0.0f},  // X
    {0.0f, 1.0f, 0.0f},  // Y
    {0.0f, 0.0f, 1.0f},  // Z
};
const int3 triangulationTable[3][4] = {
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

struct Quad {
    uint i0;
    uint i1;
    uint i2;
    uint i3;
    uint i4;
    uint i5;
    uint pad0;
    uint pad1;
};

RWStructuredBuffer<WorldVertex> vertices : register(u0);
RWStructuredBuffer<int> validity : register(u1);
AppendStructuredBuffer<Quad> indices : register(u2);

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelIndex = groupId * GROUP_OFFSET + threadId;
    float3 voxelPosition = localVoxelIndex * VOXEL_SIZE + chunkPos;
    float edgeStartDistance = GetDistance(voxelPosition, noiseTex, noiseSamp);

    for (int e = 0; e < 3; e++) {
        float3 edgeEnd = voxelPosition + triangleEdgeTable[e] * VOXEL_SIZE;
        float edgeEndDistance = GetDistance(edgeEnd, noiseTex, noiseSamp);
        if (edgeStartDistance * edgeEndDistance <= 0.0f) {
            int indiceCount = 0;
            int planeIndices[4]; 
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

                int key = (indexPosition.z) + (indexPosition.y * WORLD_RESOLUTION) + (indexPosition.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
                if (!validity[key] == -1)
                    break;

                planeIndices[t] = key;
                indiceCount++;
            }

            if (indiceCount == 4) {
                Quad newQuad;
                newQuad.i0 = 0;
                newQuad.i1 = 0;
                newQuad.i2 = 0;
                newQuad.i3 = 0;
                newQuad.i4 = 0;
                newQuad.i5 = 0;
                newQuad.pad0 = 0;
                newQuad.pad1 = 0;

                if (edgeStartDistance > edgeEndDistance) {
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
                indices.Append(newQuad);
            }
        }
    }
}

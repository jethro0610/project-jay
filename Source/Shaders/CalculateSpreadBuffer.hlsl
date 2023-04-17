#include "WorldFunctions.hlsli"
#include "GenerationConstants.hlsli"

StructuredBuffer<float2> spreadIn : register(t2);
RWStructuredBuffer<float3> spreadOut : register(u0);

[numthreads(64, 1, 1)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    uint index = groupId.x * 64 + threadId.x;

    float2 start2d = spreadIn[index];
    float3 start = float3(start2d.x, 40.0f, start2d.y);
    float3 pos = GetNearestInDirection(start, float3(0.0f, -1.0f, 0.0f), 16);
    spreadOut[index] = pos;
}

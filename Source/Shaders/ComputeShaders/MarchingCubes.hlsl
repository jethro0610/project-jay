#include "../Constants/ConstantBuffers.hlsli"
#include "../Functions/WorldFunctions.hlsli"
#include "../Constants/GenerationConstants.hlsli"

AppendStructuredBuffer<Triangle> tris : register(u0);
RWStructuredBuffer<uint> count : register(u1);

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelIndex = groupId * GROUP_OFFSET + threadId;

    float3 voxelPosition = float3(localVoxelIndex) * VOXEL_SIZE + chunkPos.xyz;
    float3 worldCornerPositions[8]; 
    float cornerDistances[8];
    uint edgeMask = 0;
    for (int i = 0; i < 8; i++) {
        worldCornerPositions[i] = voxelPosition + localCornerPositions[i] * VOXEL_SIZE;
        cornerDistances[i] = GetDistance(worldCornerPositions[i]);
        if (cornerDistances[i] < 0.0f)
            edgeMask |= 1 << i;
    }

    Triangle curTri;
    for (uint i = 0; i < 16; i++) {
        int edge = edgesFromMask[edgeMask][i]; 
        if (edge == -1)
            break;
        
        float3 corner1Pos = worldCornerPositions[edgeCorners[edge][0]];
        float3 corner2Pos = worldCornerPositions[edgeCorners[edge][1]];
        float corner1Distance = cornerDistances[edgeCorners[edge][0]];
        float corner2Distance = cornerDistances[edgeCorners[edge][1]];
        float t = -corner1Distance / (corner2Distance - corner1Distance);
        float3 vertexPos = corner1Pos + t * (corner2Pos - corner1Pos);

        float3 vertexNorm = GetNormal(vertexPos, 2.0f);

        if (i % 3 == 0) {
            curTri.v2.pos = vertexPos;
            curTri.v2.norm = vertexNorm;
        }
        else if (i % 3 == 1) {
            curTri.v1.pos = vertexPos;
            curTri.v1.norm = vertexNorm;
        }
        else {
            curTri.v0.pos = vertexPos;
            curTri.v0.norm = vertexNorm;
            tris.Append(curTri);
            InterlockedAdd(count[0], 3);
        }
    }
}

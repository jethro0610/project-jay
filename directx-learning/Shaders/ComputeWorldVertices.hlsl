static int3 cornerTable[8] = {
    { 0, 0, 0 },
    { 1, 0, 0 },
    { 1, 1, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { 1, 0, 1 },
    { 1, 1, 1 },
    { 0, 1, 1 },
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

#define WORLD_RESOLUTION 24
#define WORLD_COMPUTE_GROUPS (WORLD_RESOLUTION / 8)
#define DISTANCE_CACHE_SIZE (WORLD_RESOLUTION + 1)
#define COORDINATE_SIZE 16.0f
#define VOXEL_SIZE (COORDINATE_SIZE / (WORLD_RESOLUTION - 1))
#define GROUP_OFFSET (WORLD_RESOLUTION / WORLD_COMPUTE_GROUPS)

StructuredBuffer<float> distanceCache : register(t0);
RWStructuredBuffer<float3> computeVertices : register(u0);

float GetDistance(int3 localPosition) {
    int index = (localPosition.z) + (localPosition.y * DISTANCE_CACHE_SIZE) + (localPosition.x * DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE);
    return distanceCache[index];
}

[numthreads(8, 8, 8)]
void main(uint3 groupId : SV_GroupID, uint3 threadId : SV_GroupThreadID) {
    int3 localVoxelPosition = groupId * GROUP_OFFSET + threadId;
    int index = (localVoxelPosition.z) + (localVoxelPosition.y * WORLD_RESOLUTION) + (localVoxelPosition.x * WORLD_RESOLUTION * WORLD_RESOLUTION);
    
    float3 sumOfIntersections = float3(0.0f, 0.0f, 0.0f);
    int totalIntersections = 0;
    
    for (int e = 0; e < 12; e++) {
        int3 localEdgeStart = localVoxelPosition + cornerTable[edgeTable[e][0]];
        int3 localEdgeEnd = localVoxelPosition + cornerTable[edgeTable[e][1]];
        
        float edgeStartDistance = GetDistance(localEdgeStart);
        float edgeEndDistance = GetDistance(localEdgeEnd);
        
        // If the value is negative, it implies the two signs are different, 
        // so there must be an intersection on this edge
        if (edgeStartDistance * edgeEndDistance <= 0.0f) {
            
            float differenceRatio = edgeStartDistance / (edgeStartDistance - edgeEndDistance);
            float3 intersection = float3(0.0f, 0.0f, 0.0f);

            if (edgeStartDistance - edgeEndDistance == 0.0f)
                intersection = (float3(localEdgeStart) + float3(localEdgeEnd)) / 2.0f;
            else
                intersection = (1.0f - differenceRatio) * float3(localEdgeStart) + differenceRatio * float3(localEdgeEnd);

            sumOfIntersections += intersection;
            totalIntersections++;
        }
    }
    if (totalIntersections == 0) {
        computeVertices[index] = float3(-1.0f, 0.0f, 0.0f); // X = 1.0f indicated the vertex is non-existant
    }
    else {      
        float3 outPosition = sumOfIntersections / (float) totalIntersections;
        outPosition *= VOXEL_SIZE;
        computeVertices[index] = outPosition;
    }
}
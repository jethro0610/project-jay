Texture2D noiseTex : register(t0);
SamplerState noiseSamp;

struct TerrainModifier {
    uint type; 
    float2 position;
    float height;
    float exponent;
    float range;
};
StructuredBuffer<TerrainModifier> terrainModifiers : register(t1);

float GetNoise(float2 position) {
    float2 center = float2(0.5f, 0.5f);
    return noiseTex.SampleLevel(noiseSamp, (position / 8192.0f) * 16.0f + center, 0).r;
}

float GetTerrainHeight(float2 position) {
    float height = GetNoise(position * 0.75f) * 8.0f + 8.0f;
    height += 32.0f;
    for (int i = 0; i < 64; i++) {
        TerrainModifier modifier = terrainModifiers[i];
        float distFromModifier = distance(modifier.position, position);
        // Can probably precompute the chunk for faster performance
        if (distFromModifier > modifier.range)
            continue;

        float t = (modifier.range - distFromModifier) / modifier.range;
        float ease = t < 0.5f ? 
            pow(2.0f, modifier.exponent - 1.0f) * pow(t , modifier.exponent) : 
            1 - pow(-2 * t + 2, modifier.exponent) / 2.0f; 

        height += ease * modifier.height;
    };

    return height;
}

float GetDistance(float3 position) {
    float2 position2d = float2(position.x, position.z);

    float2 noiseDir = normalize(position2d) * 64.0f;
    noiseDir = float2(noiseDir.x, noiseDir.y);
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = GetNoise(noiseDir) * 32.0f;

    float radius = 160.0f + blobRadius;
    float height = GetTerrainHeight(position2d);

    float2 d = float2(length(position2d), abs(position.y)) - float2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    return blobDist;
}

float3 GetNormal(float3 position, float epsilon) {
    float3 dX = position + float3(epsilon, 0.0f, 0.0f);
    float3 dY = position + float3(0.0f, epsilon, 0.0f);
    float3 dZ = position + float3(0.0f, 0.0f, epsilon);

    float gradX = GetDistance(dX);
    float gradY = GetDistance(dY);
    float gradZ = GetDistance(dZ);
    return normalize(float3(gradX, gradY, gradZ));
}

float3 GetNearestInDirection(float3 start, float3 direction, uint maxSteps) {
    float3 currentPosition = start;
    for (uint i = 0; i < maxSteps; i++) {
        float distance = GetDistance(currentPosition);
        if (abs(distance) < 0.001f)
            break;
        currentPosition += direction * distance;
    }
    return currentPosition;
}

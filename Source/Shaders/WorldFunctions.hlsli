float GetNoise(float2 pos, Texture2D noiseTex, SamplerState noiseSamp) {
    float2 center = float2(0.5f, 0.5f);
    return noiseTex.SampleLevel(noiseSamp, (pos / 8192.0f) + center, 0).r;
}

float GetDistance(float3 pos, Texture2D noiseTex, SamplerState noiseSamp) {
    /* return distance(pos, float3(0.0f, 0.0f, 0.0f)) - 32.0f; */
    float2 pos2d = float2(pos.x, pos.z);
    float n = GetNoise(pos2d, noiseTex, noiseSamp);
    return pos.y - n * 4.0f;

    float2 noiseDir = normalize(pos2d) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f) {
        float blobRadius = GetNoise(noiseDir, noiseTex, noiseSamp) * 32.0f;
        /* float blobRadius = noiseTex.SampleLevel(noiseSamp, noiseDir, 0).r * 32.0f; */
    }

    float radius = 160.0f + blobRadius;
    float noiseHeight = GetNoise(pos2d, noiseTex, noiseSamp) * 8.0f + 8.0f;
    /* float noiseHeight = noiseTex.SampleLevel(noiseSamp, pos2d * 0.75f, 0).r * 8.0f + 8.0f; */
    float height = 32.0f + noiseHeight;

    float2 d = float2(length(pos2d), abs(pos.y)) - float2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    return blobDist;
}

float3 GetNormal(float3 pos, float epsilon, Texture2D noiseTex, SamplerState noiseSamp) {
    float3 dX = pos + float3(epsilon, 0.0f, 0.0f);
    float3 dY = pos + float3(0.0f, epsilon, 0.0f);
    float3 dZ = pos + float3(0.0f, 0.0f, epsilon);

    float gradX = GetDistance(dX, noiseTex, noiseSamp);
    float gradY = GetDistance(dY, noiseTex, noiseSamp);
    float gradZ = GetDistance(dZ, noiseTex, noiseSamp);
    return normalize(float3(gradX, gradY, gradZ));
}

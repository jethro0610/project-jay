float3 GetNormalFromTexture(in Texture2D normTex, in SamplerState normSampler, in float3x3 tbn, in float2 coord) {
    float3 normal = normTex.Sample(normSampler, coord);
    normal = normalize(normal * 2.0f - 1.0f);
    normal = normalize(mul(normal, tbn));
    return normal;
}

float4 GetTriPlanarColor(int texIndex, float3 position, float3 normal, float scale) {
    float3 blending = abs(normal);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= float3(b, b, b);
    
    float4 x = textures[texIndex].Sample(texSampler, position.yz * scale);
    float4 y = textures[texIndex].Sample(texSampler, position.xz * scale);
    float4 z = textures[texIndex].Sample(texSampler, position.xy * scale);
 
    return x * blending.x + y * blending.y + z * blending.z;
}

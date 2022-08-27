float4 TriPlanarColor(int texIndex, float3 position, float3 normal, float scale) {
    float3 blending = abs(normal);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= float3(b, b, b);
    
    float4 x = textures[texIndex].Sample(texSampler, position.yz * scale);
    float4 y = textures[texIndex].Sample(texSampler, position.xz * scale);
    float4 z = textures[texIndex].Sample(texSampler, position.xy * scale);
 
    return x * blending.x + y * blending.y + z * blending.z;
}

float3 TriPlanarNormal(int texIndex, float3 position, float3 normal, float scale) {
    float3 blending = abs(normal);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= float3(b, b, b);
    
    float2 uvX = position.zy * scale;
    float2 uvY = position.xz * scale;
    float2 uvZ = position.xy * scale;
    
    float3 x = textures[texIndex].Sample(texSampler, uvX).rgb;
    float3 y = textures[texIndex].Sample(texSampler, uvY).rgb;
    float3 z = textures[texIndex].Sample(texSampler, uvZ).rgb;
    
    x = float3(x.xy + normal.zy, normal.x);
    y = float3(y.xy + normal.xz, normal.y);
    z = float3(x.xy + normal.xy, normal.z);
    
    return normalize(x.zyx * blending.x + y.xzy * blending.y + z.xyz * blending.z);
}

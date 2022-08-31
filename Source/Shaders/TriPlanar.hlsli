float4 TriPlanarColor(in Texture2D colTex, in SamplerState colSampler, in float3 position, in float3 normal, in float scale) {
    float3 blending = abs(normal);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= float3(b, b, b);
    
    float4 x = colTex.Sample(colSampler, position.yz * scale);
    float4 y = colTex.Sample(colSampler, position.xz * scale);
    float4 z = colTex.Sample(colSampler, position.xy * scale);
 
    return x * blending.x + y * blending.y + z * blending.z;
}

float3 TriPlanarNormal(in Texture2D normTex, in SamplerState normSampler, in float3x3 tbn, in float3 position, in float3 normal, in float scale) {
    float3 blending = abs(normal);
    blending = normalize(max(blending, 0.00001));
    float b = (blending.x + blending.y + blending.z);
    blending /= float3(b, b, b);
    
    float2 uvX = position.zy * scale;
    float2 uvY = position.xz * scale;
    float2 uvZ = position.xy * scale;
    
    float3 x = normTex.Sample(normSampler, uvX).rgb;
    float3 y = normTex.Sample(normSampler, uvY).rgb;
    float3 z = normTex.Sample(normSampler, uvZ).rgb;
   
    x = normalize(x * 2.0f - 1.0f);
    y = normalize(y * 2.0f - 1.0f);
    z = normalize(z * 2.0f - 1.0f);

    x = float3(x.xy + normal.zy, normal.x);
    y = float3(y.xy + normal.xz, normal.y);
    z = float3(x.xy + normal.xy, normal.z);
    
    /* return normal; */
    /* return GetNormalFromTexture(normTex, normSampler, tbn, position.xz * scale); */
    return normalize(x.zyx * blending.x + y.xzy * blending.y + z.xyz * blending.z);
}

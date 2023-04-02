#include "SplatFrag.hlsli"

float4 main(SplatFrag frag) : SV_TARGET {
    if (frag.pos.x == 0.0f && frag.pos.y == 0.0f && frag.pos.z == 0.0f)
        discard; 

    float3 lightDir = float3(1.0, -1.0f, -1.0f); // TODO: Put light direction into cbuffer
    lightDir = normalize(lightDir);
    float diffuse = max(-dot(frag.norm, lightDir), 0.0f);

    return float4(1.0f, 1.0f, 1.0f, 1.0f) * diffuse;
}

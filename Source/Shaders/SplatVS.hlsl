#include "SplatVert.hlsli"
#include "SplatFrag.hlsli"
#include "ConstantBuffers.hlsli"

SplatFrag main(SplatVert vert) {
    SplatFrag output;
    if (vert.pos.x == 0.0f && vert.pos.y == 0.0f && vert.pos.z == 0.0f) {
        output.pos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        output.uv = float2(0.0f, 0.0f);
        return output;
    }

    float2 coord =float2(vert.vertexID&1, vert.vertexID>>1); 
    float3 worldPos = vert.pos + float3(coord.x, 0.0f, coord.y);

    output.pos = mul(worldViewProj, float4(worldPos, 1.0f)); 
    output.uv = coord;
    return output;
};

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
    float3 norm = normalize(vert.norm);
    float3 up = normalize(cross(norm, float3(0.0f, 1.0f, 0.0f))); 
    float3 right = normalize(cross(norm, up));
    float3 planePos;

    if (vert.vertexID == 0)
        planePos = -1.0f * right + -1.0f * up;
    else if (vert.vertexID == 1)
        planePos = 1.0f * right + -1.0f * up;
    else if (vert.vertexID == 2)
        planePos = -1.0f * right + 1.0f * up;
    else if (vert.vertexID == 3)
        planePos = 1.0f * right + 1.0f * up;

    float3 worldPos = vert.pos + planePos * 1.5f;

    output.pos = mul(worldViewProj, float4(worldPos, 1.0f)); 
    output.norm = norm;
    output.uv = coord;
    return output;
};

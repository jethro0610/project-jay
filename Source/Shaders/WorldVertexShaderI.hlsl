#include "WorldVertexInput.hlsli"
#include "WorldVertOut.hlsli"
#include "ConstantBuffers.hlsli"

WorldVertOut main(WorldVertIn inVert) {
    float2 coord = float2(inVert.vertexID&1, inVert.vertexID>>1);
    float3 worldPos = inVert.position + float3(coord.x, 0.0f, coord.y);

    WorldVertOut output;
    output.pos = mul(worldViewProj, float4(worldPos, 1.0));
    output.uv = coord;
    return output;
}

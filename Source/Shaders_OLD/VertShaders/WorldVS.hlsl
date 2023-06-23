#include "../FragTypes/WorldFrag.hlsli"
#include "../VertTypes/WorldVert.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

WorldFrag main(WorldVert vert) {
    WorldFrag frag;
    frag.position = mul(worldViewProj, float4(vert.position, 1.0));
    frag.worldPosition = float4(vert.position, 1.0f);
    frag.normal = vert.normal;
    frag.tangent = float3(0.0f, 0.0f, 0.0f);
    frag.bitangent = float3(0.0f, 0.0f, 0.0f);
    frag.uv = float2(0.0f, 0.0f);
    return frag;
}

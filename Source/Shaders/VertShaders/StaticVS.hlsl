#include "../VertTypes/StaticVert.hlsli"
#include "../FragTypes/WorldFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

WorldFrag main(StaticVert vert) {
    WorldFrag frag;
    frag.position = mul(worldViewProj, float4(vert.position, 1.0f));
    frag.worldPosition = mul(worldMat, float4(vert.position, 1.0f));
    frag.normal = normalize(mul(normalMat, vert.normal));
    frag.tangent = normalize(mul(normalMat, vert.tangent));
    frag.bitangent = normalize(mul(normalMat, vert.bitangent));
    frag.uv = float2(vert.uv.x, vert.uv.y);
    return frag;
}

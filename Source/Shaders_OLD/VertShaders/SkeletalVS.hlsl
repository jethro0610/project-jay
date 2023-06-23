#include "../VertTypes/SkeletalVert.hlsli"
#include "../FragTypes/WorldFrag.hlsli"
#include "../Constants/ConstantBuffers.hlsli"

WorldFrag main(SkeletalVert vert) {
    WorldFrag frag;

    float4x4 skinMatrix =
        vert.weights.x * jointMatrices[vert.joints.x] +
        vert.weights.y * jointMatrices[vert.joints.y] +
        vert.weights.z * jointMatrices[vert.joints.z] +
        vert.weights.w * jointMatrices[vert.joints.w];

    frag.position = mul(worldViewProj, mul(skinMatrix, float4(vert.position, 1.0f)));
    frag.worldPosition = mul(worldMat, mul(skinMatrix, float4(vert.position, 1.0f)));
    frag.normal = normalize(mul(normalMat, mul(skinMatrix,vert.normal)));
    frag.tangent = normalize(mul(normalMat, mul(skinMatrix, vert.tangent)));
    frag.bitangent = normalize(mul(normalMat, mul(skinMatrix, vert.bitangent)));
    frag.uv = float2(vert.uv.x, vert.uv.y);
    return frag;
}

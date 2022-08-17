#include "SkeletalVertexInput.hlsl"
#include "VertexOutput.hlsl"
#include "ConstantBuffers.hlsl"

VertOut main(SkeletalVertIn inVert) {
    VertOut output;

    float4x4 skinMatrix =
        inVert.weights.x * jointMatrices[inVert.joints.x] +
        inVert.weights.y * jointMatrices[inVert.joints.y] +
        inVert.weights.z * jointMatrices[inVert.joints.z] +
        inVert.weights.w * jointMatrices[inVert.joints.w];

    output.position = mul(worldViewProj, mul(skinMatrix, float4(inVert.position, 1.0)));
    output.normal = normalize(mul(normalMat, mul(skinMatrix,inVert.normal)));
    output.tangent = normalize(mul(normalMat, mul(skinMatrix, inVert.tangent)));
    output.bitangent = normalize(mul(normalMat, mul(skinMatrix, inVert.bitangent)));
    output.uv = float2(inVert.uv.x, inVert.uv.y);
    return output;
}
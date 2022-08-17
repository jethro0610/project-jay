cbuffer perObjectData {
    float4x4 worldViewProj;
    float4x4 normalMat;
};

cbuffer perSkeletonData {
    float4x4 jointMatrices[32];
};
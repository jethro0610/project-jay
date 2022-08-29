struct SkeletalVertIn {
    float3 position : POS;
    float3 normal : NORM;
    float3 tangent : TAN;
    float3 bitangent: BITAN;
    float2 uv : UV;
    int4 joints : JOINTS;
    float4 weights : WEIGHTS;
};
struct InstancedVertIn {
    float3 position : POS;
    float3 normal : NORM;
    float3 tangent : TAN;
    float3 bitangent: BITAN;
    float2 uv : UV;
    float3 instancePosition : INST_POS;
    uint instanceID : SV_InstanceID;
};

struct VertIn {
    float3 position : POS;
    float3 normal : NORM;
    float2 uv : UV;
};

struct VertOut {
    float4 position : SV_POSITION;
    float3 normal : POSITION3;
    float2 uv : TEXCOORD2;
};
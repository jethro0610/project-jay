struct VertIn {
    float3 pos : POS;
    float2 uv : UV;
};

struct VertOut {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
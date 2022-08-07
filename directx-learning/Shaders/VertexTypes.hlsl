struct VertIn {
    float3 pos : POS;
    float3 col : COL;
};

struct VertOut {
    float4 pos : SV_POSITION;
    float4 col : COLOR4;
};
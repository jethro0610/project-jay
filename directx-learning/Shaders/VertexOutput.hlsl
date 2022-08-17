struct VertOut {
    float4 position : SV_POSITION;
    float3 normal : POSITION0;
    float3 tangent : POSITION1;
    float3 bitangent : POSITION2;
    float2 uv : TEXCOORD0;
};
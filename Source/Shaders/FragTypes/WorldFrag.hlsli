struct WorldFrag {
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION0;
    float3 normal : POSITION1;
    float3 tangent : POSITION2;
    float3 bitangent : POSITION3;
    float2 uv : TEXCOORD0;
};

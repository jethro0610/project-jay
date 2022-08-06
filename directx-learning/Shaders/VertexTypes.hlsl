struct vertexIn {
    float3 positionLocal : POS;
};

struct vertexOut {
    float4 positionClip : SV_POSITION;
};
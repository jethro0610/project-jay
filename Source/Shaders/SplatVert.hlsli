struct SplatVert {
    float3 pos : INST_POS;
    float3 norm : INST_NORM;
    uint vertexID : SV_VERTEXID;
};

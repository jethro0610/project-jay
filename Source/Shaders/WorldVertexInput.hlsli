struct WorldVertIn {
    float3 position : INST_POS;
    float3 normal : INST_NORM;
    uint vertexID : SV_VERTEXID;
};

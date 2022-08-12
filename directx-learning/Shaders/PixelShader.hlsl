#include "VertexTypes.hlsl"

Texture2D tex;
SamplerState texSampler;

float4 main(VertOut outVert) : SV_TARGET{
    //return float4(outVert.uv[0], outVert.uv[1], 0.0f, 1.0f);
    //return float4(outVert.uv[0], outVert.uv[0],outVert.uv[0], 1.0f);
    float4 pixelColor = tex.Sample(texSampler, outVert.uv);
    float3 lightDir = float3(1.0f, -1.0f, 1.0f);
    float ambient = 0.2f;
    lightDir = normalize(lightDir);
    float diffuse = max(-dot(outVert.normal, lightDir), 0.0f);
    return pixelColor * (diffuse + ambient);
}
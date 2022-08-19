#include "VertexOutput.hlsl"

Texture2D textures[2];
SamplerState texSampler;

float4 main(VertOut outVert) : SV_TARGET{
    //float4 pixelColor = textures[0].Sample(texSampler, outVert.uv * 3.0f);

    float4 pixelColor = float4(0.5f, 0.1f, 0.25f, 1.0f);

    //float3 normal = textures[1].Sample(texSampler, outVert.uv * 3.0f);
    //normal = normalize(normal * 2.0f - 1.0f);
    //float3x3 textureMat = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);
    //normal = normalize(mul(normal, textureMat));
    float3 normal = outVert.normal;


    float3 lightDir = float3(0.0, -1.0f, -1.0f);
    float ambient = 0.2f;
    lightDir = normalize(lightDir);

    float diffuse = max(-dot(normal, lightDir), 0.0f);

    return pixelColor * (diffuse + ambient);
}
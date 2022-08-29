Texture2D textures[2];
SamplerState texSampler;

#include "VertexOutput.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    float4 pixelColor = textures[0].Sample(texSampler, outVert.uv);
    
    float3 normal = textures[1].Sample(texSampler, outVert.uv);
    normal = normalize(normal * 2.0f - 1.0f);
    float3x3 textureMat = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);
    normal = normalize(mul(normal, textureMat));

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    float ambient = 0.2f;
    lightDir = normalize(lightDir);
    float diffuse = max(-dot(normal, lightDir), 0.0f);

    return pixelColor * (diffuse + ambient);
}

Texture2D textures[3];
SamplerState texSampler;

#include "ConstantBuffers.hlsli"
#include "VertexOutput.hlsli"
#include "TriPlanar.hlsli"
#include "ClampedBrighten.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    const float TEXTURE_SIZE = 0.075f;
    const float MACRO_SIZE = 0.001f;
    const float MICRO_SIZE = 0.0075f;
    
    float3 worldNormal = outVert.normal;
    float3x3 tbn = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);

    float4 pixelColor = TriPlanarColor(textures[0], texSampler, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float macroStrength = TriPlanarColor(textures[2], texSampler, outVert.worldPosition.xyz, worldNormal, MACRO_SIZE).r;
    float microStrength = TriPlanarColor(textures[2], texSampler, outVert.worldPosition.xyz, worldNormal, MICRO_SIZE).r;
    float3 normal = TriPlanarNormal(textures[1], texSampler, tbn, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);

    microStrength = ClampedBrighten(microStrength, 1.0f);
    float variationStrength = microStrength * macroStrength;
    variationStrength = ClampedBrighten(variationStrength, 0.5f);

    pixelColor *= variationStrength;
    pixelColor *= 2.75f;

    float3 lightDir = float3(0.0, -1.0f, 0.0f); // TODO: Put light direction into cbuffer
    float ambient = 0.2f;
    lightDir = normalize(lightDir);

    float diffuse = max(-dot(normal, lightDir), 0.0f);

    float3 viewDir = normalize(cameraPos - outVert.worldPosition.xyz);
    float3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(-viewDir, reflectDir), 0.0f), 32);
 
    return pixelColor * (diffuse + ambient + specular);
}

Texture2D textures[3];
SamplerState texSampler;

#include "ConstantBuffers.hlsli"
#include "VertexOutput.hlsli"
#include "TriPlanar.hlsli"
#include "ClampedBrighten.hlsli"
#include "Specular.hlsli"

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

    /* pixelColor *= variationStrength; */
    /* pixelColor = ClampedBrighten(pixelColor, 0.5f); */

    float3 lightDir = float3(1.0, -1.0f, -1.0f); // TODO: Put light direction into cbuffer
    float ambient = 0.2f;
    lightDir = normalize(lightDir);

    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float specular = GetSpecular(cameraPos, outVert.worldPosition.xyz, lightDir, normal);  
    float fresnel = GetFresnel(cameraPos, outVert.worldPosition.xyz, lightDir, normal);
    /* return pixelColor; */
    return pixelColor * (diffuse + ambient + specular + fresnel);
}

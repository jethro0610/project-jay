Texture2D textures[3];
SamplerState texSampler;

#include "ConstantBuffers.hlsli"
#include "VertexOutput.hlsli"
#include "TriPlanar.hlsli"
#include "ClampedBrighten.hlsli"
#include "Specular.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    const float TEXTURE_SIZE = 0.095f;
    const float MACRO_SIZE = 0.001f;
    const float MICRO_SIZE = 0.01f;
    
    float3 worldNormal = outVert.normal;
    float3x3 tbn = float3x3(outVert.tangent, outVert.bitangent, outVert.normal);

    float4 pixelColor = TriPlanarColor(textures[0], texSampler, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float macroStrength = TriPlanarColor(textures[2], texSampler, outVert.worldPosition.xyz, worldNormal, MACRO_SIZE).r;
    float microStrength = TriPlanarColor(textures[2], texSampler, outVert.worldPosition.xyz, worldNormal, MICRO_SIZE).r;
    float3 normal = TriPlanarNormal(textures[1], texSampler, tbn, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float variationStrength = lerp(0.05f, 1.0f, (microStrength * macroStrength) + 0.35f);    
    pixelColor *= variationStrength;
    pixelColor *= 2.0f;

    float3 lightDir = float3(1.0, -1.0f, -1.0f); // TODO: Put light direction into cbuffer
    float ambient = 0.2f;
    lightDir = normalize(lightDir);
    float diffuse = max(-dot(normal, lightDir), 0.0f);
    float brightness = ambient + diffuse;

    float fresnel = GetFresnel(cameraPos, outVert.worldPosition.xyz, lightDir, normal, 1.0f, 16.0f);
    fresnel = min(fresnel, 1.0f);
    fresnel *= 0.3f;     
    float4 fresnelColor = float4(0.85, 0.9, 1.0f, 0.0f); 
    pixelColor = lerp(pixelColor, fresnelColor, fresnel);

    return pixelColor * (brightness);
}

Texture2D textures[3];
SamplerState texSampler;

#include "VertexOutput.hlsli"
#include "TriPlanar.hlsli"
#include "ClampedBrighten.hlsli"

float4 main(VertOut outVert) : SV_TARGET {
    const float TEXTURE_SIZE = 0.075f;
    const float MACRO_SIZE = 0.001f;
    const float MICRO_SIZE = 0.0075f;
    
    float3 worldNormal = outVert.normal;
    float4 pixelColor = TriPlanarColor(0, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float3 normalColor = TriPlanarNormal(1, outVert.worldPosition.xyz, worldNormal, TEXTURE_SIZE);
    float macroStrength = TriPlanarColor(2, outVert.worldPosition.xyz, worldNormal, MACRO_SIZE).r;
    float microStrength = TriPlanarColor(2, outVert.worldPosition.xyz, worldNormal, MICRO_SIZE).r;

    microStrength = ClampedBrighten(microStrength, 1.0f);
    float variationStrength = microStrength * macroStrength;
    variationStrength = ClampedBrighten(variationStrength, 0.5f);

    pixelColor *= variationStrength;
    pixelColor *= 2.75f;

    float3 lightDir = float3(1.0, -1.0f, -1.0f);
    float ambient = 0.2f;
    lightDir = normalize(lightDir);

    float diffuse = max(-dot(normalColor, lightDir), 0.0f);

    return pixelColor * (diffuse + ambient);
}

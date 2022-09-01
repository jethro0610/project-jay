#include "ScreenQuadOutput.hlsli"
Texture2D screenTex : register(t0);
SamplerState screenSampler;

float4 main(ScreenQuadOut screen) : SV_TARGET {
    int2 radius = int2(3, 3);
    float2 texelSize = float2(1.0f / 1600.0f, 1.0f / 900.0f);
    float3 kernelMean[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    float3 kernelVariance[4] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    
    int4 kernelRanges[4] = {
        {-radius.x, 0.0f, -radius.y, 0.0f},
        {0.0f, radius.x, -radius.y, 0.0f},
        {-radius.x, 0.0f, 0.0f, radius.y},
        {0.0f, radius.x, 0.0f, radius.y}
    };

    for(int i = 0; i < 4; i++) {
        int totalSamples = 0;
        for (int j = kernelRanges[i].x; j <= kernelRanges[i].y; j++) {
            for (int k = kernelRanges[i].z; k <= kernelRanges[i].w; k++) {
                float2 offset = float2(j, k) * texelSize;
                float3 color = screenTex.Sample(screenSampler, screen.coord + offset); 
                kernelMean[i] += color;
                kernelVariance[i] += color * color;
                totalSamples++;
            }
        }

        kernelMean[i] /= totalSamples;
        kernelVariance[i] = abs(kernelVariance[i] / totalSamples - kernelMean[i] * kernelMean[i]);
    }

    float3 pixelColor;
    float lowestVariance = 1.0f;
    for (int i = 0; i < 4; i++) {
        float totalVariance = kernelVariance[i].r + kernelVariance[i].g + kernelVariance[i].b;

        if (totalVariance < lowestVariance) {
            lowestVariance = totalVariance;
            pixelColor = kernelMean[i];
        }
    }

    return float4(pixelColor, 1.0f); 
}

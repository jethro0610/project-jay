$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    vec2 radius = vec2(3.0f, 3.0f);

    vec3 kernelMean[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    vec3 kernelVariance[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };
    
    vec4 kernelRanges[4] = {
        {-radius.x,     0.0f, -radius.y,     0.0f},
        {     0.0f, radius.x, -radius.y,     0.0f},
        {-radius.x,     0.0f,      0.0f, radius.y},
        {     0.0f, radius.x,      0.0f, radius.y}
    };

    for(int i = 0; i < 4; i++) {
        int totalSamples = 0;
        for (float j = kernelRanges[i].x; j <= kernelRanges[i].y; j += 1.0f) {
            for (float k = kernelRanges[i].z; k <= kernelRanges[i].w; k += 1.0f) {
                vec2 offset = vec2(j * u_viewRect.x, k * u_viewRect.y);
                vec3 color = texture2D(s_sampler0, v_texcoord0 + offset).rgb; 
                kernelMean[i] += color;
                kernelVariance[i] += color * color;
                totalSamples++;
            }
        }

        kernelMean[i] /= totalSamples;
        kernelVariance[i] = abs(kernelVariance[i] / totalSamples - kernelMean[i] * kernelMean[i]);
    }

    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    float lowestVariance = 1.0f;
    for (int i = 0; i < 4; i++) {
        float totalVariance = kernelVariance[i].r + kernelVariance[i].g + kernelVariance[i].b;

        if (totalVariance < lowestVariance) {
            lowestVariance = totalVariance;
            color = kernelMean[i];
        }
    }
    gl_FragColor = vec4(color, 1.0f);
}

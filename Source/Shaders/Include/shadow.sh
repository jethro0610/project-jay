#ifndef SHADOW_H
#define SHADOW_H

SAMPLER2D(s_samplerShadow, 8);
static const float BIAS = 0.001f;
static const float FADE_POW = 3.0f;
static const int PCF_SIZE = 1;
static const float KERNEL_SIZE = (PCF_SIZE * 2 + 1) * (PCF_SIZE * 2 + 1);

uniform vec4 u_shadowResolution;

float getShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;
    vec2 texel = 1.0f / u_shadowResolution.xy; 

    float shadow = 0.0f;
    float current = projected.z - BIAS;
    
    for (int x = -PCF_SIZE; x <= PCF_SIZE; x++)
    for (int y = -PCF_SIZE; y <= PCF_SIZE; y++){
        vec2 pos = projected.xy + vec2(x, y) * texel;
        float closest = texture2D(s_samplerShadow, pos).r;
        shadow += step(closest, current);

        // Can't figure out how to get BGFX to use white borders, using
        // this conversion for now
        // When closest = 1.0f(at the border) then border returns
        // 1.0f, which will force the shadow to be 1.0f (lit)
        // float border = step(1.0, 1.0f - closest);
    }
    shadow /= KERNEL_SIZE;

    float fade = distance(projected.xy, vec2(0.5f, 0.5f)) * 2.0f;
    fade = clamp(pow(fade, FADE_POW), 0.0f, 1.0f);
    shadow = lerp(shadow, 0.0f, fade);

    return 1.0f - shadow;
}

float getFastShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;
    
    float closest = texture2D(s_samplerShadow, projected.xy).r;
    float current = projected.z - 0.0005f;
    float shadowVal = current > closest ? 1.0f : 0.0f;

    // Can't figure out how to get BGFX to use white borders, using
    // this conversion for now
    float border = step(1.0, 1.0f - closest);
    return max(border, 1.0f - shadowVal);
}

#endif

SAMPLER2D(s_samplerShadow, 8);

uniform vec4 u_shadowResolution;

float getShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;

    vec2 texel = 1.0f / u_shadowResolution.xy; 
    float shadow = 0.0f;
    
    for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++){
        float closest = texture2D(s_samplerShadow, projected.xy + vec2(x, y) * texel).r;
        float current = projected.z - 0.0005f;
        float shadowVal = current > closest ? 1.0f : 0.0f;

        // Can't figure out how to get BGFX to use white borders, using
        // this conversion for now
        float border = step(1.0, 1.0f - closest);
        shadow += max(border, 1.0f - shadowVal);
    }
    return shadow / 9.0f;
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

SAMPLER2D(s_samplerShadow, 8);

float getShadow(vec4 sposition) {
    sposition.y = -sposition.y;
    vec3 projected = sposition.xyz / sposition.w;
    projected.xy = projected.xy * 0.5f + 0.5f;

    float closest = texture2D(s_samplerShadow, projected.xy).r;

    // Can't figure out how to get BGFX to use white borders, using
    // this conversion for now
    float border = step(1.0, 1.0f - closest);

    float current = projected.z - 0.0005f;
    float shadow = current > closest ? 1.0f : 0.0f;
    
    return max(border, 1.0f - shadow);
}

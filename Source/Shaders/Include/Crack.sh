uniform vec4 u_meter;
vec2 getCrackStrength() {
    vec2 strength = vec2(0.0f);
    float damage = 1.0f - (u_meter.x / u_meter.y);
    
    if (damage >= 0.3f)
        strength.x = 1.0f;

    if (damage >= 0.6f)
        strength.y = 1.0f;

    return strength;
}

float getSpecular(vec3 cameraPosition, vec3 wposition, vec3 lightDir, vec3 normal, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 cameraPosition, vec3 wposition, vec3 normal, float scale, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

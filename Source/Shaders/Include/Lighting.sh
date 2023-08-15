float getSpecular(vec3 cameraPosition, vec3 wposition, vec3 lightDir, vec3 normal, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 cameraPosition, vec3 wposition, vec3 normal, float scale, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

// float getDFSABrightnessNoShadow(vec3 normal, vec3 lightDirection) {
//     float ambient = 0.2f;
//     float diffuse = max(-dot(normal, lightDirection), 0.0f);
//     float brightness = diffuse + ambient;
//     float specular = getSpecular(u_cameraPosition.xyz, v_wposition, lightDirection, normal, 32.0f);
//     float fresnel = getFresnel(u_cameraPosition.xyz, v_wposition, normal, 1.0, 4.0f);
//
//     brightness = max(brightness, step(1.0f, brightness) * 1.0f);
//     brightness = max(brightness, step(0.75f, brightness) * 0.75f);
//     brightness = max(brightness, step(0.25f, brightness) * 0.25f);
//
//     // if (brightness <= 0.25f)
//     //     brightness = 0.25f;
//     // else if (brightness <= 0.75f)
//     //     brightness = 0.75f;
//     // else
//     //     brightness = 1.00f;
//
//     return brightness;
// }
//
// float getDFSABrightness(vec3 normal, vec3 lightDirection) {
//     float shadow = getShadow(v_sposition);
//     float brightness = getDFSABrightnessNoShadow(normal, lightDirection);
//
//     brightness = lerp(0.25f, brightness, shadow);
//
//     return brightness;
// }

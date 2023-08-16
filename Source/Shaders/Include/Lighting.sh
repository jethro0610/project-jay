#ifndef NOSHADOW
#include <Shadow.sh>
#endif

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

float getSpecular(vec3 cameraPosition, vec3 wposition, vec3 lightDir, vec3 normal, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(-viewDir, reflectDir), 0.0f), power);
}

float getFresnel(vec3 cameraPosition, vec3 wposition, vec3 normal, float scale, float power) {
    vec3 viewDir = normalize(cameraPosition - wposition);
    return saturate(scale * pow(1.0f + dot(-viewDir, normal), power));
}

float getDSABrightness(vec3 normal, vec3 lightDirection, vec3 cameraPosition, vec3 wPosition) {
    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = diffuse + ambient;

    #ifndef NOSPECULAR
    float specular = getSpecular(cameraPosition, wPosition, lightDirection, normal, 32.0f);
    brightness = max(brightness, step(0.3f, specular) * 1.5f);
    #endif

    brightness = max(brightness, step(0.75f, brightness) * 1.0f);
    brightness = max(brightness, step(0.25f, brightness) * 0.75f);
    brightness = max(brightness, step(0.00f, brightness) * 0.25f);

    return brightness;
}

float getBrightness(vec3 normal, vec3 lightDirection, vec3 cameraPosition, vec3 wPosition, vec4 sPosition) {
    float brightness = getDSABrightness(normal, lightDirection, cameraPosition, wPosition);

    #ifndef NOSHADOW
    float shadow = getShadow(sPosition);
    brightness = lerp(0.25f, brightness, shadow);
    #endif

    #ifndef NOFRESNEL
    float fresnel = getFresnel(cameraPosition, wPosition, normal, 1.0, 4.0f);
    brightness += fresnel;
    #endif

    return brightness;
}

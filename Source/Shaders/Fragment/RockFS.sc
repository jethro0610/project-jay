$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Crack.sh>
#include <Shadow.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler1, 1);
SAMPLER2D(s_sampler2, 2);
SAMPLER2D(s_sampler3, 3);

void main() {
    vec3 color = texture2D(s_sampler1, v_texcoord0);
    vec3 lightDirection = u_lightDirection.xyz;

    vec3 normal = texture2D(s_sampler2, v_texcoord0).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(mul(normal, v_tbn));

    float ambient = 0.2f;
    float shadow = getShadow(v_sposition);
    float diffuse = shadow * max(-dot(normal, lightDirection), 0.0f);
    float specular = shadow * getSpecular(u_cameraPosition.xyz, v_wposition, lightDirection, normal, 32.0f);
    float fresnel = getFresnel(u_cameraPosition.xyz, v_wposition, normal, 1.0, 4.0f);
    float brightness = diffuse + ambient;

    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 1.00f;

    if (specular >= 0.3f)
        brightness = 1.5f;


    vec4 crackColor = texture2DLod(s_sampler3, v_texcoord0, 0);
    vec2 crackStrength = getCrackStrength();
    float crackDepth = crackColor.r * crackStrength.x + crackColor.g * crackStrength.y;
    color = lerp(color, 0.15f, crackDepth);

    gl_FragColor = vec4(color * (brightness), 1.0f);
}

$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Shadow.sh>

uniform vec4 u_lightDirection;

void main() {
    vec3 color = vec3(0.425f, 0.5f, 0.25f) * 2.0f;
    vec3 lightDirection = u_lightDirection.xyz;

    vec3 normal = v_normal;

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = getShadow(v_sposition) * diffuse + ambient;

    if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 1.00f;

    gl_FragColor = vec4(color * brightness, 1.0f);
}

$input v_wposition, v_vposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Dither.sh>

uniform vec4 u_cameraPosition;
uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).r;
    vec3 lightDirection = u_lightDirection.xyz;

    DITHERDISCARD(1.0f - alpha);

    vec3 normal = v_normal;
    vec3 worldDirection = normalize(v_vposition - vec3(0.0f, 4.0f, 0.0f));
    normal = normalize(v_normal * 0.4f + worldDirection * 0.6f);

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = diffuse + ambient;
    brightness = max(0.5f, brightness);
    
    brightness += v_texcoord0.x * 0.15f;

    gl_FragColor = vec4(vec3(0.415f, 0.5f, 0.25f) * brightness, 1.0f);
}

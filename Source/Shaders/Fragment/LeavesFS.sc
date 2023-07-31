$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_texcoord0
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
    vec4 worldOrigin = mul(u_model[0], vec4(0.0f, 1.0f, 0.0f, 1.0f));
    vec3 worldDirection = normalize(v_wposition.xyz - worldOrigin);
    normal = normalize(v_normal * 0.3f + worldDirection * 0.7f);

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = diffuse + ambient;
    brightness = max(0.6f, brightness);
    
    brightness += v_texcoord0.x * 0.15f;

    vec3 color = vec3(1.0f, 0.95f, 0.36f) * 0.45f;
    // vec3 color = vec3(0.425f, 0.5f, 0.25f);
    gl_FragColor = vec4(color * brightness, 1.0f);
}

$input v_wposition, v_sposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Dither.sh>

SAMPLER2D(s_sampler0, 0);

void main() {
    float alpha = texture2D(s_sampler0, v_texcoord0).r;
    DITHERDISCARD(1.0f - alpha);

    vec3 lightDirection = u_lightDirection.xyz;

    vec3 normal = v_normal;

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);

    // Areas that skim the light are brightened
    diffuse = max(diffuse, pow(1.0f - abs(diffuse - 0.35f), 3.0f));

    float brightness = diffuse + ambient;
    brightness = max(0.6f, brightness);
    
    brightness += v_texcoord0.x * 0.15f;

    vec3 color = vec3(1.0f, 0.95f, 0.36f) * 0.45f;
    // vec3 color = vec3(0.425f, 0.5f, 0.25f);
    gl_FragColor = vec4(color * brightness, 1.0f);
}

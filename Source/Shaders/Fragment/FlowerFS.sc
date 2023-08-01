$input v_wposition, v_normal, v_tangent, v_bitangent, v_tbn, v_color, v_texcoord0
#include <bgfx_shader.sh>
#include <Lighting.sh>
#include <Dither.sh>

uniform vec4 u_lightDirection;

SAMPLER2D(s_sampler0, 0);

void main() {
    vec4 texColor = texture2D(s_sampler0, v_texcoord0); 
    float alpha = texColor.a;
    DITHERDISCARD(1.0f - alpha);

    vec3 color = v_color * texColor.r;
    vec3 lightDirection = u_lightDirection.xyz;

    vec3 normal = v_normal;

    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float brightness = diffuse + ambient;

    if (brightness <= 0.25f)
        brightness = 0.25f;
    else if (brightness <= 0.75f)
        brightness = 0.75f;
    else
        brightness = 1.00f;

    gl_FragColor = vec4(color * brightness, 1.0f);
}

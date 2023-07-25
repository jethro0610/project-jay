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
    vec3 worldDirection = normalize(v_vposition - vec3(0.0f, 3.0f, 0.0f));
    normal = normalize(v_normal * 0.4f + worldDirection * 0.6f);

    // normal = normal * 2.0f - 1.0f;
    // normal = normalize(mul(normal, v_tbn));
    //
    float ambient = 0.2f;
    float diffuse = max(-dot(normal, lightDirection), 0.0f);
    float specular = getSpecular(u_cameraPosition.xyz, v_wposition, lightDirection, normal, 32.0f);
    float brightness = diffuse + ambient + specular;
    brightness = max(0.5f, brightness);
    //
    // if (brightness <= 0.25f)
    //     brightness = 0.25f;
    // else if (brightness <= 0.75f)
    //     brightness = 0.75f;
    // else
    //     brightness = 1.00f;
    //
    // if (specular >= 0.3f)
    //   brightness = 1.5f;

    gl_FragColor = vec4(vec3(0.4f, 0.5f, 0.25f) * brightness, 1.0f);
    // gl_FragColor = vec4(v_texcoord0.x, v_texcoord0.x, v_texcoord0.x, 1.0f);
}

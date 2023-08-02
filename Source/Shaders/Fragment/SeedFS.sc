$input v_sposition, v_texcoord0
#include <bgfx_shader.sh>
#include <Shadow.sh>

void main() {
    vec2 origin = (0.5f, 0.5f);
    if (distance(origin, v_texcoord0) > 0.5)
        discard;
        
    gl_FragColor = max(0.5f, getShadow(v_sposition)) * vec4(1.00f, 0.86f, 0.36f, 1.0f);
}

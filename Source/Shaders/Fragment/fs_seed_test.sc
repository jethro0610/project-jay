$input v_sposition, v_texcoord0
#include <bgfx_shader.sh>

void main() {
    vec2 origin = (0.5f, 0.5f);
    float alpha = 0.5f;
    if (distance(origin, v_texcoord0) > 0.5)
        alpha = 0.0f;
        
    gl_FragColor = vec4(1.00f, 0.86f, 0.36f, alpha);
}

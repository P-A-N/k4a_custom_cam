#version 410

uniform sampler2D u_col;

in vec2 v_texcoord;

out vec4 fragcolor;


void main (void) {
    vec3 col = texture(u_col, v_texcoord*2).rgb;
    fragcolor = vec4(vec3(1000), 1.0);
}
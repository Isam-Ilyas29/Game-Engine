#version 330 core


out vec4 frag_colour;

in vec3 our_colour;
in vec2 tex_coord;

// Texture samplers
uniform sampler2D solid_texture1;

void main() {
    //frag_colour = texture(solid_texture1, tex_coord);
    frag_colour = vec4(our_colour, 1.0);
}

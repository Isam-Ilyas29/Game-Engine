#version 330 core


out vec4 FragColour;

in vec3 ourColour;
in vec2 TexCoord;

// Texture samplers
uniform sampler2D solid_texture1;
uniform sampler2D transparent_texture1;

void main() {
	// Linearly interpolate between both textures (65% container, 35% awesomeface)
	FragColour = mix(texture(solid_texture1, TexCoord), texture(transparent_texture1, TexCoord), 0.35f);
}

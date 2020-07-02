#version 330 core


out vec4 FragColour;

in vec3 ourColour;
in vec2 TexCoord;

// Texture samplers
uniform sampler2D solidTexture1;
uniform sampler2D transparentTexture1;

void main() {
	// Linearly interpolate between both textures (65% container, 35% awesomeface)
	FragColour = mix(texture(solidTexture1, TexCoord), texture(transparentTexture1, TexCoord), 0.35f);
}

#version 330 core


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    vec3 colour = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(colour, 1.f);
}
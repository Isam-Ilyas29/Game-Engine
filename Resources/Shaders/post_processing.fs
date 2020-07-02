#version 330 core


out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform uint postProcessingType;

const float offset = 1.f / 300.f;        

void main() {
    switch (postProcessingType) {
        
    case 0u:
        // Default

        FragColour = texture(screenTexture, TexCoords);
        break;

    case 1u:
        // Inverted Colours

        FragColour = vec4(vec3(1.f - texture(screenTexture, TexCoords)), 1.f);
        break;

    case 2u:
        // Greyscale

        FragColour = texture(screenTexture, TexCoords);
        float average = 0.2126f * FragColour.r + 0.7152f * FragColour.g + 0.0722f * FragColour.b;
        FragColour = vec4(average, average, average, 1.f);
        break;

    case 3u:
        // Sharpen colours
        
        vec2 sharpen_colours_offsets[9] = vec2[] (
            vec2(-offset, offset),  // Top-left
            vec2(0.f, offset),      // Top-center
            vec2(offset, offset),   // Top-right
            vec2(-offset, 0.f),     // Center-left
            vec2(0.f, 0.f),         // Center-center
            vec2(offset, 0.f),      // Center-right
            vec2(-offset, -offset), // Bottom-left
            vec2(0.f, -offset),     // Bottom-center
            vec2(offset, -offset)   // Bottom-right    
        );

        float sharpen_colours_kernel[9] = float[] (
            -1.f, -1.f, -1.f,
            -1.f,  9.f, -1.f,
            -1.f, -1.f, -1.f
        );
        
        vec3 sharpen_colours_sampleTexture[9];
        for (int i = 0; i < 9; i++) {
            sharpen_colours_sampleTexture[i] = vec3(texture(screenTexture, TexCoords.st + sharpen_colours_offsets[i]));
        }
        vec3 sharpen_colours_colour = vec3(0.f);
        for (int i = 0; i < 9; i++) {
            sharpen_colours_colour += sharpen_colours_sampleTexture[i] * sharpen_colours_kernel[i];
        }
        
        FragColour = vec4(sharpen_colours_colour, 1.f);
        break;

    case 4u:
        // Blur

        vec2 blur_offsets[9] = vec2[] (
            vec2(-offset, offset),  // Top-left
            vec2(0.f, offset),      // Top-center
            vec2(offset, offset),   // Top-right
            vec2(-offset, 0.f),     // Center-left
            vec2(0.f, 0.f),         // Center-center
            vec2(offset, 0.f),      // Center-right
            vec2(-offset, -offset), // Bottom-left
            vec2(0.f, -offset),     // Bottom-center
            vec2(offset, -offset)   // Bottom-right    
        );

        float blur_kernel[9] = float[](
            1.f / 16,  2.f / 16,  1.f / 16,
            2.f / 16,  4.f / 16,  2.f / 16,
            1.f / 16,  2.f / 16,  1.f / 16  
        );
        
        vec3 blur_sampleTexture[9];
        for (int i = 0; i < 9; i++) {
            blur_sampleTexture[i] = vec3(texture(screenTexture, TexCoords.st + blur_offsets[i]));
        }
        vec3 blur_colour = vec3(0.f);
        for (int i = 0; i < 9; i++) {
            blur_colour += blur_sampleTexture[i] * blur_kernel[i];
        }
        
        FragColour = vec4(blur_colour, 1.f);
        break;

    default:
        FragColour = vec4(1.f, 1.f, 1.f, 1.f);
        break;
    }
}

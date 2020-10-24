#version 330 core


out vec4 frag_colour;

in vec2 tex_coord;

uniform sampler2D screen_texture;
uniform uint post_processing_type;

const float offset = 1.f / 300.f;        

void main() {
    switch (post_processing_type) {
        
    case 0u:
        // Default

        frag_colour = texture(screen_texture, tex_coord);
        break;

    case 1u:
        // Inverted Colours

        frag_colour = vec4(vec3(1.f - texture(screen_texture, tex_coord)), 1.f);
        break;

    case 2u:
        // Greyscale

        frag_colour = texture(screen_texture, tex_coord);
        float average = 0.2126f * frag_colour.r + 0.7152f * frag_colour.g + 0.0722f * frag_colour.b;
        frag_colour = vec4(average, average, average, 1.f);
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
            sharpen_colours_sampleTexture[i] = vec3(texture(screen_texture, tex_coord.st + sharpen_colours_offsets[i]));
        }
        vec3 sharpen_colours_colour = vec3(0.f);
        for (int i = 0; i < 9; i++) {
            sharpen_colours_colour += sharpen_colours_sampleTexture[i] * sharpen_colours_kernel[i];
        }
        
        frag_colour = vec4(sharpen_colours_colour, 1.f);
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
            blur_sampleTexture[i] = vec3(texture(screen_texture, tex_coord.st + blur_offsets[i]));
        }
        vec3 blur_colour = vec3(0.f);
        for (int i = 0; i < 9; i++) {
            blur_colour += blur_sampleTexture[i] * blur_kernel[i];
        }
        
        frag_colour = vec4(blur_colour, 1.f);
        break;

    default:
        frag_colour = vec4(1.f, 1.f, 1.f, 1.f);
        break;
    }
}

#pragma once

#include "Core/std_types.hpp"

#include "Environment/environment.hpp"
#include "Rendering/graphic.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <vector>


class Texture {
private:
	int mWidth, mHeight, mChannels;
	u8* mData;
	GLenum mInternalFormat, mFormat;
	GLenum mID;
	std::filesystem::path mPath, mShortenedPath;
	bool mValid;

public:
	Texture(const std::filesystem::path& path);
	~Texture();

	// Prevents Texture objects from being copied 
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	GLuint dataToTextureID(u8* data, int width, int height, GLuint internal_format, GLuint type);

	void bind(u16 tex_unit) const;
	static void unbind(std::vector<u16> tex_units);

	bool isValid() const;
	unsigned char* getData() const;
	GLuint getID() const;
	GLuint getInternalFormat() const;
	GLuint getFormat() const;
	int getWidth() const;
	int getHeight() const;

	bool previewImage(GLuint* out_texture);
};

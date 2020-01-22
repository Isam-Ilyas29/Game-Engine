#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "../Environment/environment.h"

#include <iostream>

class Texture {
private:
	int mWidth, mHeight, mChannels;
	GLenum mID;
	GLenum mInternalFormat, mFormat;
	bool mValid;

public:
	Texture(const std::filesystem::path& path);
	~Texture();

	static GLuint dataToTextureID(unsigned char* data, int width, int height, GLuint internal_format, GLuint type);
	GLuint getID() const;
	void bind() const;
	void unbind() const;
	bool isValid() const;
	GLuint getFormat() const;
	int getWidth() const;
	int getHeight() const;
};


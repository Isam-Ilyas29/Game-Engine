#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "../Environment/environment.h"

#include <iostream>
#include <vector>

class Texture {
private:
	int mWidth, mHeight, mChannels;
	GLenum mID;
	GLenum mInternalFormat, mFormat;
	bool mValid;

	bool isBindCallable = true;

public:
	Texture(const std::filesystem::path& path);
	~Texture();

	static GLuint dataToTextureID(unsigned char* data, int width, int height, GLuint internal_format, GLuint type);
	GLuint getID() const;
	void bind();
	void unbind() const;
	bool isValid() const;
	GLuint getFormat() const;
	int getWidth() const;
	int getHeight() const;
};


#pragma once

#include "../Environment/environment.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <vector>

class Texture {
private:
	int mWidth, mHeight, mChannels;
	unsigned char* mData;
	GLenum mInternalFormat, mFormat;
	GLenum mID;
	bool mValid;

public:
	Texture(const std::filesystem::path& path);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture();

	static GLuint dataToTextureID(unsigned char* data, int width, int height, GLuint internal_format, GLuint type);

	void setTexture(bool bind_or_unbind, unsigned int tex_unit) const;

	bool isValid() const;

	unsigned char* getData() const;
	GLuint getID() const;
	GLuint getFormat() const;
	int getWidth() const;
	int getHeight() const;

	bool previewImage(GLuint* out_texture);
};


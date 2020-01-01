#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "environment/environment.h"

#include <iostream>

class Texture {
private:
	unsigned char* mData;
	int mWidth, mHeight;
	GLuint mID;
	GLuint mType;

public:
	Texture(unsigned char* data, int width, int height, GLuint components);
	~Texture();

	static GLuint dataToTextureID(unsigned char* data, int width, int height, GLuint type);
	GLuint getID();
	void bind();
	GLuint getType() const;
	int getWidth() const;
	int getHeight() const;
};

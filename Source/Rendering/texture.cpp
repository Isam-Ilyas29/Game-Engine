#include "Rendering/texture.hpp"

#include <fmt/format.h>

#include "Core/logger.hpp"
#include "Core/profiler.hpp"
#include "Environment/environment.hpp"

#include <string>


/*----------------------------------------------------------------------------------*/

Texture::Texture(const std::filesystem::path& path)
	: mValid(false), mWidth(0), mHeight(0), mChannels(0), mData(nullptr), mInternalFormat(0), mFormat(0), mID(0), mPath(path) {

	auto base = std::filesystem::path{ "C:/Users/ilsai/Documents/OpenGL/GladApp/Resources/" };
	auto relative = std::filesystem::relative(this->mPath, base);
	mShortenedPath = relative;

	stbi_set_flip_vertically_on_load(true);

	u8* local_data = nullptr;
	local_data = stbi_load(mPath.string().data(), &mWidth, &mHeight, &mChannels, 0);

	switch (mChannels) {
	case 3:
		mInternalFormat = GL_COMPRESSED_RGB;
		mFormat = GL_RGB;
		mValid = true;
		break;
	case 4:
		mInternalFormat = GL_COMPRESSED_RGBA;
		mFormat = GL_RGBA;
		mValid = true;
		break;
	default:
		log(logType::ERROR, fmt::format("TEXTURE ERROR | Only Support RGB & RGBA | {} Is Invalid", mPath.filename().generic_string()));
		mValid = false;
		break;
	}

	if (!(mInternalFormat && mFormat)) {
		log(logType::ERROR, fmt::format("TEXTURE ERROR | Type Not Supported | {} Is Invalid", mPath.filename().generic_string()));
		mValid = false;
	}

	if (mValid) {
		PROFILE_SCOPE((fmt::format("Texture : {} | {} * {}", mShortenedPath.generic_string(), mWidth, mHeight)).data());

		mID = dataToTextureID(local_data, mWidth, mHeight, mInternalFormat, mFormat);

		mData = local_data;
	}
}

Texture::~Texture() {
	GLAD_CHECK_ERROR(glDeleteTextures(1, &mID));
}

GLuint Texture::dataToTextureID(u8* data, int width, int height, GLuint internal_format, GLuint format) {

	GLuint textureID;

	GLAD_CHECK_ERROR(glGenTextures(1, &textureID));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, textureID));

	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data) {
		GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLAD_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));

		log(logType::INFO, fmt::format("TEXTURE | Successfully Loaded {} | Width: {}, Height: {}, Channels: {}", mShortenedPath.generic_string(), this->mWidth, this->mHeight, this->mChannels));

		stbi_image_free(data);
		data = nullptr;
	}
	else {
		log(logType::INFO, "TEXTURE ERROR | Failed To Load Texture");
	}

	return textureID;
}


void Texture::bind(u16 tex_unit) const {
	GLAD_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + tex_unit));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, getID()));
}


bool Texture::isValid() const {
	return mValid;
}
unsigned char* Texture::getData() const {
	return mData;
}
GLuint Texture::getID() const {
	if (mID > 0) {
		return mID;
	}
	return 0;
}
GLuint Texture::getInternalFormat() const {
	return mInternalFormat;
}
GLuint Texture::getFormat() const {
	return mFormat;
}
int Texture::getWidth() const {
	return mHeight;
}
int Texture::getHeight() const {
	return mWidth;
}

bool Texture::previewImage(GLuint* out_texture) {
	*out_texture = mID;

	return mValid;
}

#include "Rendering/texture.hpp"


/*----------------------------------------------------------------------------------*/

Texture::Texture(const std::filesystem::path& path)
	: mValid(false), mWidth(0), mHeight(0), mChannels(0), mData(nullptr), mInternalFormat(0), mFormat(0), mID(0) {

	stbi_set_flip_vertically_on_load(true);

	unsigned char* local_data = nullptr;
	local_data = stbi_load(path.string().c_str(), &mWidth, &mHeight, &mChannels, 0);

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
		std::cerr << "ERROR::TEXTURE::ONLY_SUPPORT_RGB_&_RGBA" << "  |  " << path.filename() << " IS INVALID." << std::endl;
		mValid = false;
		break;
	}

	if (!(mInternalFormat && mFormat)) {
		std::cerr << "ERROR::TEXTURE::TYPE_NOT_SUPPORTED" << std::endl;
		mValid = false;
	}

	if (mValid) {
		mID = dataToTextureID(local_data, mWidth, mHeight, mInternalFormat, mFormat);

		mData = local_data;
	}
}

Texture::~Texture() {
	GLAD_CHECK_ERROR(glDeleteTextures(1, &mID));
}

GLuint Texture::dataToTextureID(unsigned char* data, int width, int height, GLuint internal_format, GLuint format) {
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

		stbi_image_free(data);
		data = nullptr;
	}
	else {
		std::cerr << "Failed to load texture." << std::endl;
	}

	return textureID;
}


void Texture::bind(u16 tex_unit) const {
	GLAD_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + tex_unit));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, getID()));
}
void Texture::unbind(std::vector<u16> tex_units) {
	for (size_t i = 0; i < tex_units.size(); i++) {
		GLAD_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + i));
		GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
	}
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

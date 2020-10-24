#include "Rendering/texture.hpp"

#include "Rendering/graphic.hpp"
#include "Core/logger.hpp"

#include <stb_image.h>
#include <fmt/format.h>

#include <string>


/*----------------------------------------------------------------------------------*/

std::optional<Texture> createTexture(std::string_view string_id, const std::filesystem::path& path) {
	// Check if texture ID is valid
	for (const auto& [id, texture] : ::texture_to_id) {
		if (id == string_id) {
			log(LogType::ERROR, "TEXTURE ERROR | Duplicate texture ID provided");
			return { };
		}
	}

	// Initialise texture POD
	Texture texture;
	texture.path = path;
	texture.string_id = string_id;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* local_data = nullptr;
	local_data = stbi_load(texture.path.string().data(), &texture.width, &texture.height, &texture.channels, 0);

	// Calculate formats
	switch (texture.channels) {
	case 3:
		texture.internal_format = GL_COMPRESSED_RGB;
		texture.format = GL_RGB;
		break;
	case 4:
		texture.internal_format = GL_COMPRESSED_RGBA;
		texture.format = GL_RGBA;
		break;
	default:
		log(LogType::ERROR, fmt::format("TEXTURE ERROR | Only Support RGB & RGBA | {} Is Invalid", texture.path.filename().generic_string()));
		return { };
	}

	// Generate texture
	GLAD_CHECK_ERROR(glGenTextures(1, &texture.texture_id));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture.texture_id));

	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (local_data) {
		GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, texture.internal_format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, local_data));
		GLAD_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));

		log(LogType::INFO, fmt::format("TEXTURE | Successfully Loaded {} | Width: {}, Height: {}, Channels: {}", texture.path.generic_string(), texture.width, texture.height, texture.channels));

		stbi_image_free(local_data);
		local_data = nullptr;
	}
	else {
		log(LogType::ERROR, "TEXTURE ERROR | Failed To Load Texture");
		return { };
	}

	// Add to unordered_map after success
	::texture_to_id.emplace(string_id, texture);

	return texture;
}

void bindTexture(u16 texture_unit, Texture texture) {
	GLAD_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture.texture_id));
}

void unbindTexture(u16 texture_unit) {
	GLAD_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
}

void deleteTexture(Texture texture) {
	// Delete texture
	GLAD_CHECK_ERROR(glDeleteTextures(1, &texture.texture_id));

	// Remove from map
	for (auto it = ::texture_to_id.begin(); it != ::texture_to_id.end(); ) {
		if (it->second.texture_id == texture.texture_id) {
			it = ::texture_to_id.erase(it);
		}
		else {
			it++;
		}
	}
}

std::optional<Texture> findTexture(std::string string_id) {
	auto search = ::texture_to_id.find(string_id);
	if (search != ::texture_to_id.end()) {
		return search->second;
	}
	return { };
}


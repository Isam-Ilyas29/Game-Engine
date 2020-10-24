#pragma once

#include "Core/std_types.hpp"

#include <glad/glad.h>

#include <filesystem>
#include <unordered_map>
#include <optional>


struct Texture;

namespace {
	std::unordered_map<std::string, Texture> texture_to_id;
}

struct Texture {
	std::filesystem::path path;
	int width, height;
	int channels;
	GLenum internal_format, format;
	GLuint texture_id;
	std::string string_id;
};

std::optional<Texture> createTexture(std::string_view string_id, const std::filesystem::path& path);
void bindTexture(u16 texture_unit, Texture texture);
void unbindTexture(u16 texture_unit);
void deleteTexture(Texture texture);
std::optional<Texture> findTexture(std::string string_id);

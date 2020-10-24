#pragma once

#include "Core/std_types.hpp"
#include "Rendering/texture.hpp"
#include "Rendering/geometry.hpp"

#include <glm/glm.hpp>

#include <unordered_map>
#include <optional>


struct Mesh;

namespace {
	std::unordered_map<std::string, Mesh> mesh_to_id;
}

struct Material {
	enum Type : u32
	{
		NONE = 0,
		COLOURED = 1 << 0,
		TEXTURED = 1 << 1
	};

	Texture diffuse;
	Texture specular;
	glm::vec4 tint;
};

struct Mesh {
	Material material;

	u32 vao, vbo, ebo;
};

std::optional<Mesh> createMesh(std::string string_id, Geometry geometry, Material material);
void updateGeometry(Mesh mesh, Geometry geometry);
void deleteMesh(Mesh mesh);
std::optional<Mesh> findMesh(std::string string_id);

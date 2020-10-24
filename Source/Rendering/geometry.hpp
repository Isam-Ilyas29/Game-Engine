#pragma once

#include "Core/std_types.hpp"

#include <glm/glm.hpp>

#include <vector>


struct Vertex {
	glm::vec3 vertex_coordinate = { };
	glm::vec3 colour = { };
	glm::vec2 texure_coordinate = { };
};

struct Geometry {
	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	void addVertex(const Vertex& add_vertex);
	void addVertices(std::vector<Vertex> add_vertices);
	void addIndex(const u32& add_index);
	void addIndices(std::vector<u32> add_indices);
};

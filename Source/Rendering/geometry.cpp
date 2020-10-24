#include "Rendering/geometry.hpp"


void Geometry::addVertex(const Vertex& add_vertex) {
	vertices.push_back(add_vertex);
}
void Geometry::addVertices(std::vector<Vertex> add_vertices) {
	vertices.insert(vertices.end(), add_vertices.begin(), add_vertices.end());
}
void Geometry::addIndex(const u32& add_index) {
	indices.push_back(add_index);
}
void Geometry::addIndices(std::vector<u32> add_indices) {
	indices.insert(indices.end(), add_indices.begin(), add_indices.end());
}

#include "Rendering/mesh.hpp"

#include "Core/logger.hpp"
#include "Rendering/graphic.hpp"


std::optional<Mesh> createMesh(std::string string_id, Geometry geometry, Material material) {
	for (const auto& [id, mesh] : ::mesh_to_id) {
		if (id == string_id) {
			log(LogType::ERROR, "TEXTURE ERROR | Duplicate texture ID provided");
			return { };
		}
	}

	// Initialise mesh POD
	Mesh mesh;
	mesh.material = material;

	// Create vertex data
	GLAD_CHECK_ERROR(glGenBuffers(1, &mesh.vbo));
	GLAD_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo));
	GLAD_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), &geometry.vertices.front(), GL_STATIC_DRAW));

	if (!geometry.indices.empty()) {
		GLAD_CHECK_ERROR(glGenBuffers(1, &mesh.ebo));
		GLAD_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo));
		GLAD_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(u32), &geometry.indices.front(), GL_STATIC_DRAW));
	}

	GLAD_CHECK_ERROR(glGenVertexArrays(1, &mesh.vao));
	GLAD_CHECK_ERROR(glBindVertexArray(mesh.vao));

	//Position attribute
	GLAD_CHECK_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(0));
	// Vertex colour attributes
	GLAD_CHECK_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(1));
	//Texture co-ordinates attribute
	GLAD_CHECK_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(2));

	// Add to unordered_map after success
	::mesh_to_id.emplace(string_id, mesh);

	return mesh;
}

void updateGeometry(Mesh mesh, Geometry geometry) {
	GLAD_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo));
	GLAD_CHECK_ERROR(glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.vertices.size() * sizeof(Vertex), &geometry.vertices.front()));

	if (!geometry.indices.empty()) {
		GLAD_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo));
		GLAD_CHECK_ERROR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, geometry.indices.size() * sizeof(u32), &geometry.indices.front()));
	}

	GLAD_CHECK_ERROR(glBindVertexArray(mesh.vao));

	//Position attribute
	GLAD_CHECK_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(0));
	// Vertex colour attributes
	GLAD_CHECK_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(1));
	//Texture co-ordinates attribute
	GLAD_CHECK_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(2));
}

void deleteMesh(Mesh mesh) {
	// Remove from map
	for (auto it = ::mesh_to_id.begin(); it != ::mesh_to_id.end(); ) {
		if (it->second.vao == mesh.vao) {
			it = ::mesh_to_id.erase(it);
		}
		else {
			it++;
		}
	}
}

std::optional<Mesh> findMesh(std::string string_id) {
	auto search = ::mesh_to_id.find(string_id);
	if (search != ::mesh_to_id.end()) {
		return search->second;
	}
	return { };
}

#include "geometry_creator.h"
#include "opengl/gl_mesh.h"

void GeometryCreator::GetCube(const std::shared_ptr<GLMesh> mesh)
{
	auto &parts_mesh = mesh->GetVariablePartsMeshes()["Cube"];

	parts_mesh.vertices.resize(36);

	parts_mesh.vertices[0].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	parts_mesh.vertices[1].position = glm::vec3(0.5f, -0.5f, -0.5f);
	parts_mesh.vertices[2].position = glm::vec3(0.5f, 0.5f, -0.5f);
	parts_mesh.vertices[3].position = glm::vec3(0.5f, 0.5f, -0.5f);
	parts_mesh.vertices[4].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	parts_mesh.vertices[5].position = glm::vec3(-0.5f, -0.5f, -0.5f);

	
}

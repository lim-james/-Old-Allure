#include "Model.h"

Model::Model(const std::vector<Mesh*>& meshes) 
	: meshes(meshes) {}

Model::~Model() {
	for (const auto& mesh : meshes)
		delete mesh;

	meshes.clear();
}
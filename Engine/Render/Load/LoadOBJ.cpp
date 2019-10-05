#include "LoadOBJ.h"

#include <Math/Vectors.hpp>

#include <Logger/Logger.h>
#include <Helpers/String/StringHelpers.h>

#include <fstream>
#include <string>

Model* Load::OBJ(const std::string& filepath) {

	if (modelCache.find(filepath) != modelCache.end())
		return modelCache[filepath];

	std::ifstream ifs(filepath);

	std::string groupName;
	std::string buffer;

	std::vector<vec3f> positions;
	std::vector<vec2f> texCoords;
	std::vector<vec3f> normals;

	vec2f v2Buffer;
	vec3f v3Buffer;

	Vertex vertexBuffer;

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	while (!ifs.eof()) {
		std::getline(ifs, buffer);
		auto list = Helpers::Split(buffer, ' ');
		
		if (list.size() > 0) {
			if (list[0] == "g") {
				list.erase(list.begin());
				groupName = Helpers::Join(list, ' ');
			} else if (list[0] == "v") {
				v3Buffer.Set(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2]),
					Helpers::Parse<float>(list[3])
				);
				positions.push_back(v3Buffer);
			} else if (list[0] == "vt") {
				v2Buffer.Set(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2])
				);
				texCoords.push_back(v2Buffer);
			} else if (list[0] == "vn") {
				v3Buffer.Set(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2]),
					Helpers::Parse<float>(list[3])
				);
				normals.push_back(v3Buffer);
			} else if (list[0] == "f") {
				std::vector<int> listBuffer;

				const auto& order = list.size() == 5 ? quadIndices : triangleIndices;

				for (const auto& i : order) {
					listBuffer = Helpers::Split<int>(list[i], '/');

					vertexBuffer = {
						positions[listBuffer[0] - 1],
						normals[listBuffer[2] - 1],
						texCoords[listBuffer[1] - 1]
					};

					const auto position = std::find(vertices.begin(), vertices.end(), vertexBuffer);

					if (position == vertices.end()) {
						const unsigned index = vertices.size();
						vertices.push_back(vertexBuffer);
						indices.push_back(index);
					} else {
						indices.push_back(std::distance(vertices.begin(), position));
					}
				}
			}
		}
	}

	Mesh* mesh = new Mesh(vertices, indices);
	Model* model = new Model({ mesh });
	modelCache[filepath] = model;

	return model;
}

void Load::ClearModelCache() {
	for (const auto& model : modelCache)
		delete model.second;

	modelCache.clear();
}
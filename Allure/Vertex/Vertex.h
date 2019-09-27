#ifndef VERTEX_H
#define VERTEX_H

#include "../Math/Vectors.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

struct Vertex {

	vec3f position;
	vec4f color;

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

};

#endif
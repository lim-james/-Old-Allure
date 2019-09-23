#ifndef SHADER_H
#define SHADER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <tuple>

class Shader {

	VkDevice* device;
	
	VkShaderModule shaderModule;
	VkPipelineShaderStageCreateInfo shaderStageInfo;

public:

	Shader();

	bool Create(const char* filepath, const VkShaderStageFlagBits& stage, VkDevice& ref);
	void Destroy();

	const VkPipelineShaderStageCreateInfo& GetStageInfo() const;

private:

	std::vector<char> ReadFile(const char* filepath);
	bool CreateShaderModule(const std::vector<char>& code);

};

#endif

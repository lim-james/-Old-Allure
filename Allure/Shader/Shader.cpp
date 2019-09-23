#include "Shader.h"

#include "../Logger/Logger.h"

#include <fstream>

Shader::Shader() 
	: device(nullptr) 
	, shaderModule(VK_NULL_HANDLE) {
	shaderStageInfo = {};
}

bool Shader::Create(const char* filepath, const VkShaderStageFlagBits& stage, VkDevice& ref) {
	device = &ref;

	const auto shaderCode = ReadFile(filepath);

	if (!CreateShaderModule(shaderCode)) 
		return false;

	shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = "main";

	return true;
}

void Shader::Destroy() {
	vkDestroyShaderModule(*device, shaderModule, nullptr);
}

const VkPipelineShaderStageCreateInfo& Shader::GetStageInfo() const {
	return shaderStageInfo;
}

std::vector<char> Shader::ReadFile(const char* filepath) {
	std::ifstream ifs(filepath, std::ios::ate | std::ios::binary);

	if (!ifs) {
		Console::Error << "Failed to open " << filepath << '\n';
		return {};
	}

	const size_t fileSize = static_cast<size_t>(ifs.tellg());
	std::vector<char> buffer(fileSize);

	ifs.seekg(0);
	ifs.read(buffer.data(), fileSize);

	ifs.close();

	return buffer;
}

bool Shader::CreateShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		Console::Error << "Failed to create shader module.\n";
		return false;
	}

	return true;
}
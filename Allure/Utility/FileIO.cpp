#include "FileIO.h"

#include "../Logger/Logger.h"

#include <fstream>
#include <iostream>

std::vector<char> Utility::ReadFile(const char* filepath) {
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

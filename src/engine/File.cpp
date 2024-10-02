#include "engine/File.hpp"

std::string File::readFile(const std::string& filePath) {
	std::ifstream file(filePath);
	NON_CRITICAL_ASSERT(file.is_open(), "File " + filePath + " can't be opened");

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
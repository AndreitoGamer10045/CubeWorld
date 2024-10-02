#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "engine/DebugHelp.hpp"

namespace File {
	std::string readFile(const std::string& filePath);
};
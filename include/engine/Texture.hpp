#pragma once

#include <string>

#include "glad/glad.h"

#include "stb_image/stb_image.h"

#include "engine/DebugHelp.hpp"

class Texture
{
public:
	Texture(std::string textureFile, int textureWrapX, int textureWrapY, int textureMinFilter, int textureMaxFilter, bool isGeneratingMipmaps);
	~Texture();

	void bind();
	void activate(unsigned int location);

	unsigned int getId();

private:
	unsigned int m_id;
};
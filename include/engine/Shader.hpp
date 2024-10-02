#pragma once

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "engine/File.hpp"

class Shader
{
public:
	Shader(int type, const std::string& shaderFile);
	~Shader();

	void compile();

	unsigned int getId();
	int getType();

private:
	unsigned int m_id;
	std::string m_code;
	int m_type;
};


class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void attachShader(Shader& shader);

	void link();
	void use();

	unsigned int getId();

	unsigned int getUniformLocation(const char* uniformName);
	void setUniformInt(const char* uniformName, int i);
	void setUniformFloat(const char* uniformName, float f);
	void setUniformMatrix4(const char* uniformName, const glm::mat4& matrix);
	void setUniformVector3(const char* uniformName, const glm::vec3& vector);


private:
	unsigned int m_id;
	bool m_used;
};
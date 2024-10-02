#include "engine/Shader.hpp"

Shader::Shader(int type, const std::string& shaderFile)
{
	m_type = type;
	m_code = File::readFile(shaderFile);
	m_id = glCreateShader(type);
}

Shader::~Shader()
{
	glDeleteShader(m_id);
}


void Shader::compile()
{
	const char* shaderCode = m_code.c_str();
	glShaderSource(m_id, 1, &shaderCode, NULL);
	glCompileShader(m_id);

	int  success;
	char infoLog[512];
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(m_id, 512, NULL, infoLog);
		std::cout << (m_type ? "Vertex" : "Fragment") << " shader compilation failed:" << infoLog << std::endl;
	}
}


unsigned int Shader::getId()
{
	return m_id;
}

int Shader::getType()
{
	return m_type;
}


ShaderProgram::ShaderProgram()
{
	m_id = glCreateProgram();
	m_used = false;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}



void ShaderProgram::attachShader(Shader& shader)
{
	glAttachShader(m_id, shader.getId());
}


void ShaderProgram::link()
{
	glLinkProgram(m_id);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "Program linking failed:\n" << infoLog << std::endl;
	}
}

void ShaderProgram::use()
{
	glUseProgram(m_id);
	m_used = true;
}


unsigned int ShaderProgram::getId()
{
	return m_id;
}


unsigned int ShaderProgram::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(m_id, uniformName);
}


void ShaderProgram::setUniformInt(const char* uniformName, int i)
{
	CRITICAL_ASSERT(m_used, "Cannot give an uniform when a shader program is not used");
	glUniform1i(getUniformLocation(uniformName), i);
}

void ShaderProgram::setUniformFloat(const char* uniformName, float f)
{
	glUniform1f(getUniformLocation(uniformName), f);
}

void ShaderProgram::setUniformMatrix4(const char* uniformName, const glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniformVector3(const char* uniformName, const glm::vec3& vector)
{
	glUniform3fv(getUniformLocation(uniformName), 1, glm::value_ptr(vector));
}
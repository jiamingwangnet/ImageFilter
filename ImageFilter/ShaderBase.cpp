#include "include/ShaderBase.h"
#include <glad/gl.h>
#include <stdexcept>
#include <iostream>

void ShaderBase::Use() const
{
	glUseProgram(id);
}

void ShaderBase::Unuse() const
{
	glUseProgram(0);
}

void ShaderBase::SetInt(const std::string& name, int value) const
{
	glProgramUniform1i(id, GetUniformLoc(name), value);
}

void ShaderBase::SetFloat(const std::string & name, float value) const
{
	glProgramUniform1f(id, GetUniformLoc(name), value);
}

void ShaderBase::SetVec2(const std::string & name, const glm::vec2 & value) const
{
	glProgramUniform2fv(id, GetUniformLoc(name), 1, &value[0]);
}

void ShaderBase::SetVec3(const std::string & name, const glm::vec3 & value) const
{
	glProgramUniform3fv(id, GetUniformLoc(name), 1, &value[0]);
}

void ShaderBase::SetVec4(const std::string & name, const glm::vec4 & value) const
{
	glProgramUniform4fv(id, GetUniformLoc(name), 1, &value[0]);
}

void ShaderBase::SetMat2(const std::string & name, const glm::mat2 & value) const
{
	glProgramUniformMatrix2fv(id, GetUniformLoc(name), 1, GL_FALSE, &value[0][0]);
}

void ShaderBase::SetMat3(const std::string & name, const glm::mat3 & value) const
{
	glProgramUniformMatrix3fv(id, GetUniformLoc(name), 1, GL_FALSE, &value[0][0]);
}

void ShaderBase::SetMat4(const std::string & name, const glm::mat4 & value) const
{
	glProgramUniformMatrix4fv(id, GetUniformLoc(name), 1, GL_FALSE, &value[0][0]);
}

unsigned int ShaderBase::GetUniformLoc(const std::string& name) const
{
	GLint loc = glGetUniformLocation(id, name.c_str());
	if (loc == GL_INVALID_INDEX)
	{
		std::cerr << ((std::string)"Uniform " + name + " not found in shader.") << std::endl;
		throw std::runtime_error{ (std::string)"Uniform " + name + " not found in shader." };
	}
	return (unsigned int)loc;
}
#include "include/ShaderBase.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
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

std::string ShaderBase::MetaGetName(const std::string& source)
{
	std::stringstream sSource{ source };

	bool startfound = false;
	for (std::string line; std::getline(sSource, line); )
	{
		if (line.find("//metastart") != std::string::npos)
			startfound = true;
		else if (line.find("//metaend") != std::string::npos)
			break;

		if (startfound)
		{
			std::stringstream sToken{ line };
			std::vector<std::string> tokens;
			std::string token;
			while (std::getline(sToken, token, ':'))
				tokens.push_back(token);

			if (tokens[0] == "//name")
			{
				tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), ' '), tokens[1].end()); // remove all spaces
				return tokens[1];
			}
		}
	}

	return "";
}

int ShaderBase::MetaGetUniformInt(const std::string& source, const std::string& name)
{
	ValueTypes type;
	std::string value;
	ParseUniform(source, name, type, value);

	return std::stoi(value);
}

float ShaderBase::MetaGetUniformFloat(const std::string& source, const std::string& name)
{
	ValueTypes type;
	std::string value;
	ParseUniform(source, name, type, value);

	return std::stof(value);
}

bool ShaderBase::ParseUniform(const std::string& source, const std::string& name, ValueTypes& type_out, std::string& value_out)
{
	std::stringstream sSource{ source };

	bool startfound = false;
	for (std::string line; std::getline(sSource, line); )
	{
		if (line.find("//metastart") != std::string::npos)
			startfound = true;
		else if (line.find("//metaend") != std::string::npos)
			break;

		if (startfound)
		{
			std::stringstream sToken{ line };
			std::vector<std::string> tokens;
			std::string token;
			while (std::getline(sToken, token, ':'))
				tokens.push_back(token);

			std::size_t pos = tokens[0].find("//uniform<");
			if (pos != std::string::npos)
			{
				std::size_t end = tokens[0].find('>', pos);
				std::string typeStr = tokens[0].substr(pos + 10, end - pos - 10);

				std::string fname = tokens[0].substr(end + 2, tokens[0].length() - end - 2);
				if (fname != name) continue;

				tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), ' '), tokens[1].end());

				if (typeStr == "int")
				{
					type_out = ValueTypes::Int;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "float")
				{
					type_out = ValueTypes::Float;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "vec2")
				{
					type_out = ValueTypes::Vec2;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "vec3")
				{
					type_out = ValueTypes::Vec3;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "vec4")
				{
					type_out = ValueTypes::Vec4;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "mat2")
				{
					type_out = ValueTypes::Mat2;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "mat3")
				{
					type_out = ValueTypes::Mat3;
					value_out = tokens[1];
					return true;
				}
				else if (typeStr == "mat4")
				{
					type_out = ValueTypes::Mat4;
					value_out = tokens[1];
					return true;
				}
			}
		}
	}

	return false; // not found
}
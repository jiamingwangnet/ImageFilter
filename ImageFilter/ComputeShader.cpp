#include "include/ComputeShader.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

ComputeShader::ComputeShader(const std::string& path)
	: path{path}
{
	Build();
}

void ComputeShader::Build()
{
	std::ifstream file{ path };
	std::stringstream sourceStream;
	sourceStream << file.rdbuf();

	source = sourceStream.str();
	const char* cstr = source.c_str();

	unsigned int compute;
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cstr, nullptr);
	glCompileShader(compute);

	// check errors
	int success;
	char infoLog[512];

	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, compute);
	glLinkProgram(id);

	// check errors
	success = 0;
	memset(infoLog, 0, sizeof(infoLog));

	glGetShaderiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}

	glDeleteShader(compute);

	// get uniforms

	int count;
	int size;
	GLenum type;

	const GLsizei bufSize = 64;
	char name[bufSize] = { 0 };
	GLsizei length;

	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(id, i, bufSize, &length, &size, &type, name);

		ValueTypes vtype;

		switch (type)
		{
		case GL_INT:
			vtype = ValueTypes::Int;
			break;
		case GL_FLOAT:
			vtype = ValueTypes::Float;
			break;
		case GL_FLOAT_VEC2:
			vtype = ValueTypes::Vec2;
			break;
		case GL_FLOAT_VEC3:
			vtype = ValueTypes::Vec3;
			break;
		case GL_FLOAT_VEC4:
			vtype = ValueTypes::Vec4;
			break;
		case GL_FLOAT_MAT2:
			vtype = ValueTypes::Mat2;
			break;
		case GL_FLOAT_MAT3:
			vtype = ValueTypes::Mat3;
			break;
		case GL_FLOAT_MAT4:
			vtype = ValueTypes::Mat4;
			break;
		default:
			vtype = ValueTypes::Other;
			break;
		}

		uniforms.insert({ (std::string)name, vtype });

		memset(name, 0, sizeof(name));
	}
}

std::string ComputeShader::MetaGetName(const std::string& source)
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

int ComputeShader::MetaGetUniformInt(const std::string& source, const std::string& name)
{
	ValueTypes type;
	std::string value;
	ParseUniform(source, name, type, value);

	return std::stoi(value);
}

float ComputeShader::MetaGetUniformFloat(const std::string& source, const std::string& name)
{
	ValueTypes type;
	std::string value;
	ParseUniform(source, name, type, value);

	return std::stof(value);
}

bool ComputeShader::ParseUniform(const std::string& source, const std::string& name, ValueTypes& type_out, std::string& value_out)
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

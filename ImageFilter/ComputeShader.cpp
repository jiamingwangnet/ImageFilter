#include "include/ComputeShader.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

ComputeShader::ComputeShader(const std::string& path)
	: ShaderBase{path}
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

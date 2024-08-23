#include "include/ShaderProgram.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertpath, const std::string& fragpath)
{
	std::ifstream vertFile{ vertpath };
	std::ifstream fragFile{ fragpath };

	std::stringstream vertStream, fragStream;

	vertStream << vertFile.rdbuf();
	fragStream << fragFile.rdbuf();

	std::string vertStr, fragStr;
	vertStr = vertStream.str();
	fragStr = fragStream.str();

	Build(vertStr.c_str(), fragStr.c_str());
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
{
	Build(vertexSource, fragmentSource);
}

void ShaderProgram::Build(const char* vertSource, const char* fragSource)
{
	unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, nullptr);
	glCompileShader(vert);

	// log errors
	int success;
	char infoLog[512];

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}

	unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, nullptr);
	glCompileShader(frag);

	// log errors
	success = 0;
	memset(infoLog, 0, sizeof(infoLog));

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vert);
	glAttachShader(id, frag);
	glLinkProgram(id);

	// log errors
	success = 0;
	memset(infoLog, 0, sizeof(infoLog));

	glGetShaderiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}
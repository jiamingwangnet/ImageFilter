#pragma once

#include "ShaderBase.h"
#include <string>

class ShaderProgram : public ShaderBase
{
public:
	ShaderProgram() = default;
	ShaderProgram(const std::string& vertpath, const std::string& fragpath);
	ShaderProgram(const char* vertexSource, const char* fragmentSource);

private:
	void Build(const char* vertSource, const char* fragSource);
};
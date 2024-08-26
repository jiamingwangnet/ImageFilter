#pragma once

#include "ShaderBase.h"
#include <string>
#include <stdexcept>

class ShaderProgram : public ShaderBase
{
public:
	ShaderProgram() = default;
	ShaderProgram(const std::string& vertpath, const std::string& fragpath);
	ShaderProgram(const char* vertexSource, const char* fragmentSource);

	void Build(const char* vertSource, const char* fragSource);
	void Build() override { throw std::runtime_error{ "DO NOT USE." }; };
};
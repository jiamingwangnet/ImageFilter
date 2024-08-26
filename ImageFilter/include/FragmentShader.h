#pragma once

#include <string>
#include "ShaderBase.h"

class FragmentShader : public ShaderBase
{
public:
	FragmentShader(const std::string& path); // compile shader

	void Build() override;
};
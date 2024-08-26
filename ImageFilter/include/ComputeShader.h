#pragma once

#include <string>
#include <unordered_map>
#include "ShaderBase.h"
#include <glm/glm.hpp>

// shaders are created using the shader manager
class ComputeShader : public ShaderBase
{
public:
	ComputeShader(const std::string& path); // compile shader

	void Build() override;
};
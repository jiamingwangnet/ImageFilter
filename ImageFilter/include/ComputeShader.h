#pragma once

#include <string>
#include <unordered_map>
#include "ShaderBase.h"
#include <glm/glm.hpp>

// shaders are created using the shader manager
class ComputeShader : public ShaderBase
{
public:
	enum class ValueTypes
	{
		Int, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Other
	};
public:
	ComputeShader(const std::string& path); // compile shader

	void Build();
	const std::unordered_map<std::string, ValueTypes>& GetUniforms() { return uniforms; }
	const std::string& GetSource() const { return source; }

	// retrieve metadata
	/*
		format:
			label: value

		default uniform values format:
			uniform<type>_[uniform name]: value
	*/
	static std::string MetaGetName(const std::string& source);

	static int MetaGetUniformInt(const std::string& source, const std::string& name);
	static float MetaGetUniformFloat(const std::string& source, const std::string& name);
	static glm::vec2 MetaGetUniformVec2(const std::string& source, const std::string& name);
	static glm::vec3 MetaGetUniformVec3(const std::string& source, const std::string& name);
	static glm::vec4 MetaGetUniformVec4(const std::string& source, const std::string& name);
	static glm::mat2 MetaGetUniformMat2(const std::string& source, const std::string& name);
	static glm::mat3 MetaGetUniformMat3(const std::string& source, const std::string& name);
	static glm::mat4 MetaGetUniformMat4(const std::string& source, const std::string& name);

private:
	static bool ParseUniform(const std::string& source, const std::string& name, ValueTypes& type_out, std::string& value_out);

private:
	std::string path;
	std::string source;

	std::unordered_map<std::string, ValueTypes> uniforms; // stores the uniforms as a pair of {name, type}
};
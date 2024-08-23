#pragma once

#include <glm/glm.hpp>
#include <string>

// determines the uniforms value at runtime
union UniformValue
{
	int intv;
	float floatv;
	glm::vec2 vec2;
	glm::vec3 vec3;
	glm::vec4 vec4;
	glm::mat2 mat2;
	glm::mat3 mat3;
	glm::mat4 mat4;
};

// holds a shader program's ID and provides functions such as uniform access functions
class ShaderBase
{
public:
	ShaderBase() = default;

	void Use() const;
	void Unuse() const;

	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetMat2(const std::string& name, const glm::mat2& value) const;
	void SetMat3(const std::string& name, const glm::mat3& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
	unsigned int GetUniformLoc(const std::string& name) const;

protected:
	unsigned int id;
};
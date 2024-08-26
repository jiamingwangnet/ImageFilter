#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

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
	enum class ValueTypes
	{
		Int, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Other
	};

public:
	ShaderBase() = default;
	ShaderBase(const std::string& path) : path{ path }, source{}, id{ (unsigned int)-1 }, uniforms{} {};

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

	virtual void Build() = 0;
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

protected:
	static bool ParseUniform(const std::string& source, const std::string& name, ValueTypes& type_out, std::string& value_out);

private:
	unsigned int GetUniformLoc(const std::string& name) const;

protected:
	unsigned int id;

	std::string path;
	std::string source;

	std::unordered_map<std::string, ValueTypes> uniforms; // stores the uniforms as a pair of {name, type}
};
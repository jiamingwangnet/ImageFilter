#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "ComputeShader.h"
#include <stdexcept>

// initialises shaders as unique pointers and puts them in a map with a string as the key
class ShaderManager
{
public:
	enum class SHADER_TYPE
	{
		FRAGMENT,
		COMPUTE
	};

	static void CreateShader(const std::string& path, const std::string& name, SHADER_TYPE shaderType);

	template<class T>
	static T& GetShader(const std::string& name);

private:
	static std::unordered_map<std::string, std::unique_ptr<ShaderBase>> storage;
};

template<class T>
T& ShaderManager::GetShader(const std::string& name)
{
	try
	{
		return *static_cast<T*>(storage.at(name).get());
	}
	catch (const std::out_of_range& error)
	{
		throw std::out_of_range{ (std::string)"Can't find shader " + name };
	}
}

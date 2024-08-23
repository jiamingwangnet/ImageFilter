#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "ComputeShader.h"

// initialises shaders as unique pointers and puts them in a map with a string as the key
// only manages compute shaders
class ShaderManager
{
public:
	static void CreateShader(const std::string& path, const std::string& name);
	static ComputeShader& GetShader(const std::string& name);

private:
	static std::unordered_map<std::string, std::unique_ptr<ComputeShader>> storage;
};
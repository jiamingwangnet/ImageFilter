#include "include/ShaderManager.h"
#include "include/FragmentShader.h"

std::unordered_map<std::string, std::unique_ptr<ShaderBase>> ShaderManager::storage;

void ShaderManager::CreateShader(const std::string& path, const std::string& name, SHADER_TYPE shaderType)
{
	switch (shaderType)
	{
	case SHADER_TYPE::COMPUTE:
		storage.insert({ name, std::make_unique<ComputeShader>(path) });
		break;
	case SHADER_TYPE::FRAGMENT:
		storage.insert({ name, std::make_unique<FragmentShader>(path) });
		break;
	}
}

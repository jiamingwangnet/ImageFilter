#include "include/ImageManager.h"
#include "include/ShaderManager.h"
#include "include/ComputeShader.h"
#include <stb/stb_image.h>
#include <glad/gl.h>

void ImageManager::Load(const std::string& path)
{
	// TODO: delete old textures if any

	int bpp;

	unsigned char* data = stbi_load(path.c_str(), &(original.width), &(original.height), &bpp, 0);

	original.aspect = (float)original.width / (float)original.height;
	edited.width = original.width;
	edited.height = original.height;
	edited.aspect = original.aspect;

	glGenTextures(1, &(original.texture));
	glActiveTexture(GL_TEXTURE0); // bind to unit 0
	glBindTexture(GL_TEXTURE_2D, original.texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, original.width, original.height, 0, bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);


	glGenTextures(1, &(edited.texture));
	glActiveTexture(GL_TEXTURE1); // bind to unit 1
	glBindTexture(GL_TEXTURE_2D, edited.texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, edited.width, edited.height, 0, bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

void ImageManager::ApplyEffect(const std::string& name)
{
	const ComputeShader& shader = ShaderManager::GetShader(name);

	glBindImageTexture(0, original.texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, edited.texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	shader.Use();
	glDispatchCompute((GLuint)original.width, (GLuint)original.height, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // make sure writing to image has finished
}

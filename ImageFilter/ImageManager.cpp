#include "include/ImageManager.h"
#include "include/ShaderManager.h"
#include "include/ComputeShader.h"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <GLFW/glfw3.h>

void ImageManager::Load(const std::string& path)
{
	if (original.texture != (unsigned int)-1)
	{
		glDeleteTextures(1, &(original.texture));
		glDeleteTextures(1, &(edited.texture));
	}

	int bpp;

	unsigned char* data = stbi_load(path.c_str(), &(original.width), &(original.height), &bpp, 0);
	if (data == nullptr)
		return;

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

void ImageManager::CamCapture(int device)
{
	if (!camera.isOpen())
	{
		camera.Open(device);

		original.width = camera.GetWidth();
		original.height = camera.GetHeight();

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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, edited.width, edited.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);


		glGenTextures(1, &(edited.texture));
		glActiveTexture(GL_TEXTURE1); // bind to unit 1
		glBindTexture(GL_TEXTURE_2D, edited.texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, edited.width, edited.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}

	camera.Capture(original.texture);
	camera.Capture(edited.texture);
}

void ImageManager::ApplyEffect(const std::string& name)
{
	ComputeShader& shader = ShaderManager::GetShader<ComputeShader>(name);

	glBindImageTexture(0, original.texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, edited.texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	shader.Use();

	shader.SetWillThrow(false);
	shader.SetFloat(Application_TimeName, glfwGetTime());
	shader.SetWillThrow(true);

	glDispatchCompute((GLuint)original.width, (GLuint)original.height, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // make sure writing to image has finished
}

void ImageManager::SaveOutput(const std::string& path)
{
	std::vector<unsigned char> image(3 * edited.width * edited.height);

	glActiveTexture(GL_TEXTURE1);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());

	stbi_write_png(path.c_str(), edited.width, edited.height, 3, image.data(), 3 * edited.width);
}

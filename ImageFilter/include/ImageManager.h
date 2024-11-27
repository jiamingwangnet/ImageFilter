#pragma once

#include <string>
#include "CameraHandler.h"
#include <glad/gl.h>

// stores the image textures required
class ImageManager
{
public:
	struct Image
	{
		unsigned int texture = (unsigned int)-1;
		int width;
		int height;
		float aspect;
	};
public:
	ImageManager() = default;
	void Load(const std::string& path); // create a texture
	void CamCapture(int device);
	void CloseCamera() { camera.Close(); };

	void ApplyEffect(const std::string& name);
	void SaveOutput(const std::string& path);

	const Image& GetOriginalImage() const { return original; }
	const Image& GetEditedImage() const { return edited; }

	void BeginCamThread() { camera.BeginReadBuf(); }
	void EndCamThread() { camera.EndReadBuf(); }

	void UpscaleImage(bool v) { 
		if (v == camera.isUpscaling()) return;
		camera.UpscaleImage(v); 
		glDeleteTextures(1, &(original.texture));
		glDeleteTextures(1, &(edited.texture));
		original.texture = -1;
		edited.texture = -1;

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
private:
	// only able to modify one image at a time
	Image original = {}; // compute shader reads from this image (unit 0)
	Image edited = {}; // compute shader writes to this image (unit 1)

	CameraHandler camera;
};
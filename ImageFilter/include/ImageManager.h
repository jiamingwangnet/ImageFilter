#pragma once

#include <string>
#include "CameraHandler.h"

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
private:
	// only able to modify one image at a time
	Image original = {}; // compute shader reads from this image (unit 0)
	Image edited = {}; // compute shader writes to this image (unit 1)

	CameraHandler camera;
};
#pragma once

#include <string>

// stores the image textures required
class ImageManager
{
public:
	struct Image
	{
		unsigned int texture;
		int width;
		int height;
		float aspect;
	};
public:
	ImageManager() = default;
	void Load(const std::string& path); // create a texture

	void ApplyEffect(const std::string& name);

	const Image& GetOriginalImage() const { return original; }
	const Image& GetEditedImage() const { return edited; }
private:
	// only able to modify one image at a time
	Image original = {}; // compute shader reads from this image (unit 0)
	Image edited = {}; // compute shader writes to this image (unit 1)
};
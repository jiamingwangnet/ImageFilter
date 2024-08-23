#pragma once

#include "ImageManager.h"
#include "ShaderProgram.h"

class Window;

// manages the rendering of the image
class Renderer
{
private:
	// image position should be defined in pixel space coordinates and then translated into screen space using transformation matrices
	struct Vertex
	{
		float x; // in screen space coordinates
		float y;
		float z;
		float u;
		float v;
	};

	// defines a square ranging from screenspace -0.5 to 0.5
	static constexpr Vertex vertices[6] = {
		Vertex{-0.5f, -0.5f, 0.0f,  0.0f, 0.0f},
		Vertex{-0.5f,  0.5f, 0.0f,  0.0f, 1.0f},
		Vertex{ 0.5f,  0.5f, 0.0f,  1.0f, 1.0f},

		Vertex{ 0.5f, -0.5f, 0.0f,  1.0f, 0.0f},
		Vertex{-0.5f, -0.5f, 0.0f,  0.0f, 0.0f},
		Vertex{ 0.5f,  0.5f, 0.0f,  1.0f, 1.0f},
	};

	static constexpr const char* vertexShader =
		R"GLSL(
	#version 460 core	

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 uv;

	uniform mat4 model;
	uniform mat4 projection;

	out vec2 fragUV;
	
	void main()
	{
		gl_Position = projection * model * vec4(position.xyz, 1.0);
		fragUV = uv;
	}
	)GLSL";

	static constexpr const char* fragmentShader =
		R"GLSL(
	#version 460 core	

	uniform sampler2D editedImage;

	in vec2 fragUV;

	out vec4 FragColor;
	
	void main()
	{
		FragColor = texture(editedImage, fragUV);
	}
	)GLSL";

public:
	Renderer() = default;

	void Init(const Window* window);
	void RenderImage(const Window& window, const ImageManager::Image& image);

	void SetZoom(float value) { zoom = value; }
	void IncZoom(float value) { zoom += value; }

	void SetPosition(glm::vec2 value) { position = value; }
	void IncPosition(glm::vec2 value) { position += value; }

private:
	unsigned int vao = 0;
	unsigned int vbo = 0;

	ShaderProgram shader;

	glm::mat4 projection;

	glm::vec2 position{ 0.0f,0.0f };
	float zoom = 1.0f;
};
#include "include/Renderer.h"
#include "include/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include "include/InputManager.h"
#include <iostream>

void Renderer::Init(const Window* window)
{
	shader = ShaderProgram{ vertexShader, fragmentShader };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	InputManager::BindScroll([this, window](double x, double y)
		{
			if (!(window->GetGUI().GetIO()->WantCaptureMouse))
			{
				this->IncZoom(y * 0.1);
			}
		}
	);

	InputManager::BindCursor([this, window](double x, double y)
		{
			if (glfwGetMouseButton(window->GetWindow(), GLFW_MOUSE_BUTTON_1) && !(window->GetGUI().GetIO()->WantCaptureMouse))
			{
				this->IncPosition(glm::vec2{ InputManager::GetCursorXDelta() * 0.003f, InputManager::GetCursorYDelta() * 0.003f }); // TODO: change to not use delta
			}
		}
	);
}

void Renderer::RenderImage(const Window& window, const ImageManager::Image& image)
{
	float winWidth = (float)window.GetWidth();
	float winHeight = (float)window.GetHeight();

	const float size = 1.0f / 500.0f;

	projection = glm::ortho(-(winWidth / 2.0f) * size, winWidth / 2.0f * size, winHeight / 2.0f * size, -(winHeight / 2.0f) * size, -1000.0f, 1000.0f); // sets 0, 0 to the middle

	glm::mat4 model{ 1.0f };

	model = glm::translate(model, glm::vec3{ position.x, position.y, 0.0f});

	model = glm::scale(model, glm::vec3{ image.aspect * zoom, zoom, zoom });

	glBindVertexArray(vao);
	
	shader.Use();
	shader.SetMat4("model", model);
	shader.SetMat4("projection", projection);
	shader.SetInt("editedImage", 1); // set to output unit 1

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	shader.Unuse();
}
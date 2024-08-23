#include "include/Window.h"
#include <glad/gl.h>

int main()
{
	Window window{ 1280, 720 };

	window.Create();
	window.MainLoop();

	return 0;
}

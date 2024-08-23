#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <functional>

class InputManager
{
public:
	using scrollfunc = std::function<void(double, double)>;
	using cursorfunc = std::function<void(double, double)>;
	using mousebtnfunc = std::function<void(int, int, int)>;
public:
	static void BindCallbacks(GLFWwindow* window);

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void BindScroll(scrollfunc func)
	{
		scrollcallback = func;
	}

	static void BindMouseButton(mousebtnfunc func)
	{
		mousebtncallback = func;
	}

	static void BindCursor(cursorfunc func)
	{
		cursorcallback = func;
	}

	static double GetCursorX() { return cursorX; }
	static double GetCursorY() { return cursorY; }

	static double GetCursorXDelta() { return cursorX - lastCurX; }
	static double GetCursorYDelta() { return cursorY - lastCurY; }
private:
	static scrollfunc scrollcallback;
	static mousebtnfunc mousebtncallback;
	static cursorfunc cursorcallback;

	static double cursorX;
	static double cursorY;

	static double lastCurX;
	static double lastCurY;
};
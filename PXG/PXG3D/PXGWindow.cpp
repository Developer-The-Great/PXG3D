#include <GLFW/glfw3.h>
#include "Input.h"
#include "PXGWindow.h"
#include "Debug.h"
#include <iostream>

namespace PXG
{
	namespace PXGWindow
	{
		void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
		{
			Input::mouse_button_callback(window,button,action,mods);
		}
		void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			Input::key_callback(window, key, scancode, action, mods);
		}
		void mouse_position_callback(GLFWwindow * window, double xpos, double ypos)
		{
			Input::mouse_position_callback(window, xpos, ypos);
		}
		GLFWwindow* MakeGLFWWindow(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share)
		{
			return glfwCreateWindow(width, height, "PXG3D", NULL, NULL);
		}
		void Init()
		{
			glfwInit();
			//set configurations(what kind of window we want to make)
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			//we get a smaller subset of OPENGL features
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			//make a glfw window
		}
	}
}

#include <GLFW/glfw3.h>
#include <iostream>
#include "context.h"
#include <cstdlib>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

std::unique_ptr<Context> contextPtr;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "UselessEngine", nullptr, nullptr);

	glfwSetKeyCallback(window, key_callback);

	contextPtr.reset(new Context());
	contextPtr->Create();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		contextPtr->Update();
	}

	contextPtr->Destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
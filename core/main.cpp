#include <GLFW/glfw3.h>
#include <iostream>
#include "context.h"
#include <cstdlib>

void size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

std::unique_ptr<Context> context;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "UselessEngine", nullptr, nullptr);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, size_callback);

	context = std::make_unique<Context>(window);
	context->Create();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		int width = 0, height = 0;
		glfwGetFramebufferSize(context->window, &width, &height);
		if (width == 0 || height == 0) {
			glfwWaitEvents();
			continue;
		}

		context->Update();
	}

	context->Destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void size_callback(GLFWwindow* window, int width, int height)
{
	context->OnSizeCallback();
}
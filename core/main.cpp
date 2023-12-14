#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include "context.h"

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "UselessEngine", nullptr, nullptr);

	Context context;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		context.Update();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
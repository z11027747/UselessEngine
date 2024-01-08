#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include "context.h"

void size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

constexpr int width = 1920;
constexpr int height = 1080;
static float aspect = (width * 1.0f) / height;

std::unique_ptr<Context> context;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, "UselessEngine", nullptr, nullptr);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, size_callback);

	context = std::make_unique<Context>(window, aspect);
	context->Create();

	auto deltaTime = 0.0f;
	auto lastTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		auto width = 0, height = 0;
		glfwGetFramebufferSize(context->window, &width, &height);
		if (width == 0 || height == 0)
		{
			glfwWaitEvents();
			continue;
		}

		auto currTime = static_cast<float>(glfwGetTime());
		deltaTime = currTime - lastTime;
		lastTime = currTime;

		context->currTime = currTime;
		context->deltaTime = deltaTime;
		context->Update();
	}

	context->Destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	context->OnKeyCallback(key, action, mode);
}

void size_callback(GLFWwindow *window, int width, int height)
{
	context->OnSizeCallback();
}
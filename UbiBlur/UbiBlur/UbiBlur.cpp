// UbiBlur.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <Renderer.hpp>

static void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) {
		return;
	}

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	Engine::Renderer *renderer = static_cast<Engine::Renderer*>(glfwGetWindowUserPointer(window));

	switch (key) {
	case GLFW_KEY_B: renderer->setBlurEnabled(!renderer->isBlurEnabled()); break;
	case GLFW_KEY_1: renderer->setShadingModel(Engine::Renderer::ShadingModel::CookTorrance); break;
	case GLFW_KEY_2: renderer->setShadingModel(Engine::Renderer::ShadingModel::BlinnPhong); break;
	default: break;
	}
}

int main(int argc, char* argv[]) {
	filesystem::path path(argv[0]);
	Engine::Size2D rtSize(1280, 720);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(rtSize.width, rtSize.height, "Blur", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	Engine::Renderer renderer(path.parent_path(), rtSize);
	glfwSetWindowUserPointer(window, &renderer);

	while (!glfwWindowShouldClose(window)) {
		renderer.render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
#include "Window.h"

namespace dubu::window {

Window::Window() {
	glfwInit();
	mWindow = glfwCreateWindow(800, 600, "dubu-engine", nullptr, nullptr);
}

Window::~Window() {
	if (mWindow) {
		glfwDestroyWindow(mWindow);
	}
	glfwTerminate();
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(mWindow);
}
void Window::PollEvents() {
	glfwPollEvents();
}

}  // namespace dubu::window
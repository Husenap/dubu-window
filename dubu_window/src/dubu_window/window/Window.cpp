#include "Window.h"

#include "Events.h"

namespace dubu::window {

int Window::ConstructionCounter = 0;

Window::Window()
    : Window(800, 600, "dubu-window") {}

Window::Window(int width, int height, const std::string& title) {
	if (ConstructionCounter <= 0) {
		glfwInit();
	}
	++ConstructionCounter;

	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(mWindow, this);

	glfwSetFramebufferSizeCallback(mWindow, WindowFramebufferSizeCallback);
	glfwSetKeyCallback(mWindow, WindowKeyCallback);
	glfwSetDropCallback(mWindow, WindowDropCallback);
	glfwSetWindowContentScaleCallback(mWindow, WindowContentScaleCallback);
}

Window::~Window() {
	if (mWindow) {
		glfwDestroyWindow(mWindow);
	}

	--ConstructionCounter;
	if (ConstructionCounter <= 0) {
		glfwTerminate();
	}
}

void Window::PollEvents() {
	glfwPollEvents();
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(mWindow);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(mWindow);
}

Window* Window::GetUserWindow(GLFWwindow* window) {
	return reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
}

void Window::WindowFramebufferSizeCallback(GLFWwindow* window,
                                           int         width,
                                           int         height) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventResize{.width = width, .height = height});
	}
}

void Window::WindowKeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventKey{
		    .key = key, .scancode = scancode, .action = action, .mods = mods});

		switch (action) {
		case GLFW_PRESS:
			userWindow->Emit(
			    EventKeyPress{.key = key, .scancode = scancode, .mods = mods});
			break;
		case GLFW_RELEASE:
			userWindow->Emit(EventKeyRelease{
			    .key = key, .scancode = scancode, .mods = mods});
			break;
		case GLFW_REPEAT:
			userWindow->Emit(
			    EventKeyRepeat{.key = key, .scancode = scancode, .mods = mods});
			break;
		default:
			break;
		}
	}
}

void Window::WindowDropCallback(GLFWwindow*  window,
                                int          count,
                                const char** paths) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		for (int i = 0; i < count; ++i) {
			userWindow->Emit(EventDroppedFile{.file = paths[i]});
		}
	}
}

void Window::WindowContentScaleCallback(GLFWwindow* window,
                                        float       scaleX,
                                        float       scaleY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventContentScale{scaleX, scaleY});
	}
}

}  // namespace dubu::window
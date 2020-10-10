#include "Window.h"

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
	glfwSetCharCallback(mWindow, WindowCharCallback);
	glfwSetCursorPosCallback(mWindow, WindowCursorPosCallback);
	glfwSetCursorEnterCallback(mWindow, WindowCursorEnterCallback);
	glfwSetMouseButtonCallback(mWindow, WindowMouseButtonCallback);
	glfwSetScrollCallback(mWindow, WindowScrollCallback);
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

void Window::SetCursorMode(dubu::window::CursorMode cursorMode) {
	mCursorMode = cursorMode;

	switch (mCursorMode) {
	case CursorMode::Normal:
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case CursorMode::Hidden:
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case CursorMode::Locked:
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	}
}

bool Window::IsHovered() const {
	return glfwGetWindowAttrib(mWindow, GLFW_HOVERED);
}

void Window::SimulateEventKey(const EventKey& e) {
	Emit(e);

	switch (e.action) {
	case GLFW_PRESS:
		Emit(EventKeyPress{
		    .key = e.key, .scancode = e.scancode, .mods = e.mods});
		break;
	case GLFW_RELEASE:
		Emit(EventKeyRelease{
		    .key = e.key, .scancode = e.scancode, .mods = e.mods});
		break;
	case GLFW_REPEAT:
		Emit(EventKeyRepeat{
		    .key = e.key, .scancode = e.scancode, .mods = e.mods});
		break;
	default:
		break;
	}
}

void Window::SimulateEventChar(const EventChar& e) {
	Emit(e);
}

void Window::SimulateEventCursorPos(const EventCursorPos& e) {
	Emit(e);
}

void Window::SimulateEventMouseButton(const EventMouseButton& e) {
	Emit(e);

	switch (e.action) {
	case GLFW_PRESS:
		Emit(EventMouseButtonPress{.button = e.button, .mods = e.mods});
		break;
	case GLFW_RELEASE:
		Emit(EventMouseButtonRelease{.button = e.button, .mods = e.mods});
		break;
	default:
		break;
	}
}

bool Window::IsGamepadConnected(int gamepadIndex) {
	return glfwJoystickIsGamepad(gamepadIndex);
}

std::optional<GLFWgamepadstate> Window::GetGamepadState(int gamepadIndex) {
	if (!IsGamepadConnected(gamepadIndex)) {
		return std::nullopt;
	}

	GLFWgamepadstate gps;

	if (glfwGetGamepadState(gamepadIndex, &gps) == GLFW_FALSE) {
		return std::nullopt;
	}

	return gps;
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
		userWindow->Emit(EventContentScale{.scaleX = scaleX, .scaleY = scaleY});
	}
}

void Window::WindowCharCallback(GLFWwindow* window, uint32_t codepoint) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventChar{.codepoint = codepoint});
	}
}

void Window::WindowCursorPosCallback(GLFWwindow* window,
                                     double      posX,
                                     double      posY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventCursorPos{.posX = posX, .posY = posY});
	}
}

void Window::WindowCursorEnterCallback(GLFWwindow* window, int entered) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		if (entered) {
			userWindow->Emit<EventCursorEnter>();
		} else {
			userWindow->Emit<EventCursorLeave>();
		}
	}
}

void Window::WindowMouseButtonCallback(GLFWwindow* window,
                                       int         button,
                                       int         action,
                                       int         mods) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(
		    EventMouseButton{.button = button, .action = action, .mods = mods});

		switch (action) {
		case GLFW_PRESS:
			userWindow->Emit(
			    EventMouseButtonPress{.button = button, .mods = mods});
			break;
		case GLFW_RELEASE:
			userWindow->Emit(
			    EventMouseButtonRelease{.button = button, .mods = mods});
			break;
		default:
			break;
		}
	}
}

void Window::WindowScrollCallback(GLFWwindow* window,
                                  double      offsetX,
                                  double      offsetY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventScroll{.offsetX = offsetX, .offsetY = offsetY});
	}
}

}  // namespace dubu::window
#include "GLFWWindow.h"

namespace dubu::window {

int GLFWWindow::ConstructionCounter = 0;

GLFWWindow::GLFWWindow()
    : GLFWWindow(800, 600, "dubu-window") {}

GLFWWindow::GLFWWindow(int width, int height, const std::string& title) {
	if (ConstructionCounter <= 0) {
		glfwInit();
	}
	++ConstructionCounter;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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

GLFWWindow::~GLFWWindow() {
	if (mWindow) {
		glfwDestroyWindow(mWindow);
	}

	--ConstructionCounter;
	if (ConstructionCounter <= 0) {
		glfwTerminate();
	}
}

void GLFWWindow::PollEvents() {
	glfwPollEvents();
}

void GLFWWindow::SwapBuffers() {
	glfwSwapBuffers(mWindow);
}

bool GLFWWindow::ShouldClose() const {
	return glfwWindowShouldClose(mWindow);
}

bool GLFWWindow::IsHovered() const {
	return glfwGetWindowAttrib(mWindow, GLFW_HOVERED);
}

bool GLFWWindow::IsFocused() const {
	return glfwGetWindowAttrib(mWindow, GLFW_FOCUSED);
}

void GLFWWindow::SetCursorMode(dubu::window::CursorMode cursorMode) {
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

bool GLFWWindow::IsGamepadConnected(int gamepadIndex) {
	return glfwJoystickIsGamepad(gamepadIndex);
}

std::optional<GLFWgamepadstate> GLFWWindow::GetGamepadState(int gamepadIndex) {
	if (!IsGamepadConnected(gamepadIndex)) {
		return std::nullopt;
	}

	GLFWgamepadstate gps;

	if (glfwGetGamepadState(gamepadIndex, &gps) == GLFW_FALSE) {
		return std::nullopt;
	}

	return gps;
}

GLFWWindow* GLFWWindow::GetUserWindow(GLFWwindow* window) {
	return reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
}

void GLFWWindow::WindowFramebufferSizeCallback(GLFWwindow* window,
                                               int         width,
                                               int         height) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventResize{.width = width, .height = height});
	}
}

void GLFWWindow::WindowKeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		switch (action) {
		case GLFW_PRESS:
			userWindow->Emit(EventKey{.key      = static_cast<Key>(key),
			                          .scancode = scancode,
			                          .action   = Action::Press,
			                          .mods     = static_cast<Mods>(mods)});
			userWindow->Emit(EventKeyPress{.key      = static_cast<Key>(key),
			                               .scancode = scancode,
			                               .mods = static_cast<Mods>(mods)});
			break;
		case GLFW_RELEASE:
			userWindow->Emit(EventKey{.key      = static_cast<Key>(key),
			                          .scancode = scancode,
			                          .action   = Action::Release,
			                          .mods     = static_cast<Mods>(mods)});
			userWindow->Emit(EventKeyRelease{.key      = static_cast<Key>(key),
			                                 .scancode = scancode,
			                                 .mods = static_cast<Mods>(mods)});
			break;
		case GLFW_REPEAT:
			userWindow->Emit(EventKey{.key      = static_cast<Key>(key),
			                          .scancode = scancode,
			                          .action   = Action::Repeat,
			                          .mods     = static_cast<Mods>(mods)});
			userWindow->Emit(EventKeyRepeat{.key      = static_cast<Key>(key),
			                                .scancode = scancode,
			                                .mods = static_cast<Mods>(mods)});
			break;
		default:
			break;
		}
	}
}

void GLFWWindow::WindowDropCallback(GLFWwindow*  window,
                                    int          count,
                                    const char** paths) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		for (int i = 0; i < count; ++i) {
			userWindow->Emit(EventDroppedFile{.file = paths[i]});
		}
	}
}

void GLFWWindow::WindowContentScaleCallback(GLFWwindow* window,
                                            float       scaleX,
                                            float       scaleY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventContentScale{.scaleX = scaleX, .scaleY = scaleY});
	}
}

void GLFWWindow::WindowCharCallback(GLFWwindow* window, uint32_t codepoint) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventChar{.codepoint = codepoint});
	}
}

void GLFWWindow::WindowCursorPosCallback(GLFWwindow* window,
                                         double      posX,
                                         double      posY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventCursorPos{.posX = posX, .posY = posY});
	}
}

void GLFWWindow::WindowCursorEnterCallback(GLFWwindow* window, int entered) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		if (entered) {
			userWindow->Emit<EventCursorEnter>();
		} else {
			userWindow->Emit<EventCursorLeave>();
		}
	}
}

void GLFWWindow::WindowMouseButtonCallback(GLFWwindow* window,
                                           int         button,
                                           int         action,
                                           int         mods) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		switch (action) {
		case GLFW_PRESS:
			userWindow->Emit(
			    EventMouseButton{.button = static_cast<MouseButton>(button),
			                     .action = Action::Press,
			                     .mods   = static_cast<Mods>(mods)});
			userWindow->Emit(EventMouseButtonPress{
			    .button = static_cast<MouseButton>(button),
			    .mods   = static_cast<Mods>(mods)});
			break;
		case GLFW_RELEASE:
			userWindow->Emit(
			    EventMouseButton{.button = static_cast<MouseButton>(button),
			                     .action = Action::Release,
			                     .mods   = static_cast<Mods>(mods)});
			userWindow->Emit(EventMouseButtonRelease{
			    .button = static_cast<MouseButton>(button),
			    .mods   = static_cast<Mods>(mods)});
			break;
		default:
			break;
		}
	}
}

void GLFWWindow::WindowScrollCallback(GLFWwindow* window,
                                      double      offsetX,
                                      double      offsetY) {
	if (auto userWindow = GetUserWindow(window); userWindow) {
		userWindow->Emit(EventScroll{.offsetX = offsetX, .offsetY = offsetY});
	}
}

}  // namespace dubu::window
#pragma once

#include "Events.h"

namespace dubu::window {

enum class CursorMode {
	Normal,
	Hidden,
	Locked,
};

class Window : public dubu::event::EventEmitter {
public:
	Window();
	Window(int width, int height, const std::string& title);
	~Window();

	void PollEvents();
	bool ShouldClose() const;
	void SwapBuffers();

	void SetCursorMode(dubu::window::CursorMode cursorMode);
	bool IsHovered() const;

	void SimulateEventKey(const EventKey& e);
	void SimulateEventChar(const EventChar& e);
	void SimulateEventCursorPos(const EventCursorPos& e);
	void SimulateEventMouseButton(const EventMouseButton& e);

	static bool                            IsGamepadConnected(int gamepadIndex);
	static std::optional<GLFWgamepadstate> GetGamepadState(int gamepadIndex);

private:
	static Window* GetUserWindow(GLFWwindow* window);
	static void    WindowFramebufferSizeCallback(GLFWwindow* window,
	                                             int         width,
	                                             int         height);
	static void    WindowKeyCallback(
	       GLFWwindow* window, int key, int scancode, int action, int mods);
	static void WindowDropCallback(GLFWwindow*  window,
	                               int          count,
	                               const char** paths);
	static void WindowContentScaleCallback(GLFWwindow* window,
	                                       float       scaleX,
	                                       float       scaleY);
	static void WindowCharCallback(GLFWwindow* window, uint32_t codepoint);
	static void WindowCursorPosCallback(GLFWwindow* window,
	                                    double      posX,
	                                    double      posY);
	static void WindowCursorEnterCallback(GLFWwindow* window, int entered);
	static void WindowMouseButtonCallback(GLFWwindow* window,
	                                      int         button,
	                                      int         action,
	                                      int         mods);
	static void WindowScrollCallback(GLFWwindow* window,
	                                 double      offsetX,
	                                 double      offsetY);

	static int ConstructionCounter;

	GLFWwindow* mWindow;
	CursorMode  mCursorMode;
};

}  // namespace dubu::window
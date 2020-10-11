#pragma once

#include "IWindow.h"

namespace dubu::window {

class GLFWWindow : public IWindow {
public:
	GLFWWindow();
	GLFWWindow(int width, int height, const std::string& title);
	~GLFWWindow();

	void PollEvents() override;
	void SwapBuffers() override;

	[[nodiscard]] bool ShouldClose() const override;
	[[nodiscard]] bool IsHovered() const override;
	[[nodiscard]] bool IsFocused() const override;

	void SetCursorMode(dubu::window::CursorMode cursorMode) override;

	[[nodiscard]] bool IsGamepadConnected(int gamepadIndex) override;
	[[nodiscard]] std::optional<GamepadState> GetGamepadState(
	    int gamepadIndex) override;

	[[nodiscard]] GLFWwindow* GetGLFWHandle() const { return mWindow; }

private:
	static GLFWWindow* GetUserWindow(GLFWwindow* window);
	static void        WindowFramebufferSizeCallback(GLFWwindow* window,
	                                                 int         width,
	                                                 int         height);
	static void        WindowKeyCallback(
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
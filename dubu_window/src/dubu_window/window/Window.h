#pragma once

namespace dubu::window {

class Window : public dubu::event::EventEmitter {
public:
	Window();
	Window(int width, int height, const std::string& title);
	~Window();

	void PollEvents();
	bool ShouldClose() const;
	void SwapBuffers();

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

	GLFWwindow* mWindow;
	static int  ConstructionCounter;
};

}  // namespace dubu::window
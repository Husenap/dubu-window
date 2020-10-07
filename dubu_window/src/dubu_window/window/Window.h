#pragma once

namespace dubu::window {
class Window {
public:
	Window();
	~Window();

	void PollEvents();
	bool ShouldClose() const;

private:
	GLFWwindow* mWindow;
};
}  // namespace dubu::window
#include <iostream>
#include <string>
#include <thread>

#include <dubu_window/dubu_window.h>

struct WindowInstance {
	std::unique_ptr<dubu::window::Window> window;
	dubu::event::Token                    resizeToken;
};

int main() {
	constexpr int NumWindows = 5;

	std::vector<WindowInstance> windows;
	for (int i = 0; i < NumWindows; ++i) {
		windows.emplace_back(
		    WindowInstance{.window = std::make_unique<dubu::window::Window>(
		                       400, 400, "Window " + std::to_string(i + 1))});
	}

	std::vector<dubu::event::Token> tokens;

	for (std::size_t i = 0; i < windows.size(); ++i) {
		windows[i].resizeToken =
		    windows[i].window->Subscribe<dubu::window::EventResize>(
		        [i](const auto& e) {
			        std::cout << "Window " << (i + 1) << " resized: " << e.width
			                  << ", " << e.height << ")" << std::endl;
		        });
	}

	while (!windows.empty()) {
		for (std::size_t i = 0; i < windows.size(); ++i) {
			auto& window = windows[i].window;
			if (window->ShouldClose()) {
				windows.erase(windows.begin() + i);
				--i;
				continue;
			}

			window->PollEvents();
		}

		std::this_thread::yield();
	}
}

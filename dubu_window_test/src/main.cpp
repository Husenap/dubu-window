#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

#include <dubu_window/dubu_window.h>

class WindowInstance : public dubu::event::EventSubscriber {
public:
	WindowInstance(int number) {
		name   = "Window " + std::to_string(number);
		window = std::make_unique<dubu::window::Window>(400, 400, name);

		Subscribe<dubu::window::EventResize>(
		    [this](const auto& e) {
			    std::cout << name << " resized: (" << e.width << ", "
			              << e.height << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventContentScale>(
		    [this](const auto& e) {
			    std::cout << name << " content scale: (" << e.scaleX << ", "
			              << e.scaleY << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventKeyPress>(
		    [this](const auto& e) {
			    std::cout << name << " key press: (" << e.key << ", "
			              << e.scancode << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventKey>(
		    [this](const auto& e) {
			    std::cout << name << " key: (" << e.key << ", " << e.scancode
			              << ", " << e.action << ", " << e.mods << ")"
			              << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventKeyRelease>(
		    [this](const auto& e) {
			    std::cout << name << " key release: (" << e.key << ", "
			              << e.scancode << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventKeyRepeat>(
		    [this](const auto& e) {
			    std::cout << name << " key repeat: (" << e.key << ", "
			              << e.scancode << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventChar>(
		    [this](const auto& e) {
			    std::cout << name << " char: (" << e.codepoint << ")"
			              << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventCursorPos>(
		    [this](const auto& e) {
			    std::cout << name << " cursor pos: (" << e.posX << ", "
			              << e.posY << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventCursorEnter>(
		    [this](const auto&) {
			    std::cout << name << " cursor enter" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventCursorLeave>(
		    [this](const auto&) {
			    std::cout << name << " cursor leave" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventMouseButton>(
		    [this](const auto& e) {
			    std::cout << name << " mouse button: (" << e.button << ", "
			              << e.action << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventMouseButtonPress>(
		    [this](const auto& e) {
			    std::cout << name << " mouse button press: (" << e.button
			              << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventMouseButtonRelease>(
		    [this](const auto& e) {
			    std::cout << name << " mouse button release: (" << e.button
			              << ", " << e.mods << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventScroll>(
		    [this](const auto& e) {
			    std::cout << name << " scroll: (" << e.offsetX << ", "
			              << e.offsetY << ")" << std::endl;
		    },
		    *window);
		Subscribe<dubu::window::EventDroppedFile>(
		    [this](const auto& e) {
			    std::cout << name << " dropped file: (" << e.file << ")"
			              << std::endl;
		    },
		    *window);
	}
	std::string                           name;
	std::unique_ptr<dubu::window::Window> window;
};

int main() {
	constexpr int                                NumStartWindows = 2;
	int                                          windowCount     = 0;
	std::vector<std::unique_ptr<WindowInstance>> windows;

	auto AddNewWindow = [&] {
		windows.emplace_back(std::make_unique<WindowInstance>(++windowCount));
	};

	for (int i = 0; i < NumStartWindows; ++i) {
		AddNewWindow();
	}

	while (!windows.empty()) {
		for (std::size_t i = 0; i < windows.size(); ++i) {
			auto& window = windows[i]->window;
			if (window->ShouldClose()) {
				windows.erase(windows.begin() + static_cast<std::ptrdiff_t>(i));
				--i;
				continue;
			}

			window->PollEvents();
		}

		std::this_thread::yield();
	}
}

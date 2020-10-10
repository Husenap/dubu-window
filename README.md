![Windows](https://github.com/Husenap/dubu-window/workflows/Windows/badge.svg)
![Ubuntu](https://github.com/Husenap/dubu-window/workflows/Ubuntu/badge.svg)

```
   _     _                 _       _           
 _| |_ _| |_ _ _ ___ _ _ _|_|___ _| |___ _ _ _ 
| . | | | . | | |___| | | | |   | . | . | | | |
|___|___|___|___|   |_____|_|_|_|___|___|_____|
```

C++ Window Library using [GLFW](https://github.com/glfw/glfw) and [dubu-event](https://github.com/Husenap/dubu-event/).

# Features

* Open multiple windows
* Subscribe to window events
* Simulate input events

# Examples

##### **`Simple Window`**
```cpp
#include <iostream>

#include <dubu_window/dubu_window.h>

int main() {
    dubu::window::Window window(400, 400, "simple window");
    auto                 token =
        window.RegisterListener<dubu::window::EventResize>([](const auto& e) {
            std::cout << "Window "
                      << " resized: " << e.width << ", " << e.height << ")"
                      << std::endl;
        });

    while (!window.ShouldClose()) {
        window.PollEvents();
    }
}
```

##### **`Multiple Windows`**
```cpp
#include <iostream>
#include <string>

#include <dubu_window/dubu_window.h>

struct WindowInstance {
    std::unique_ptr<dubu::window::Window> window;
    dubu::event::Token                    resizeToken;
};

int main() {
    constexpr int NumWindows = 2;

    std::vector<WindowInstance> windows;
    for (int i = 0; i < NumWindows; ++i) {
        windows.emplace_back(
            WindowInstance{.window = std::make_unique<dubu::window::Window>(
                               400, 400, "Window " + std::to_string(i + 1))});
    }

    std::vector<dubu::event::Token> tokens;

    for (std::size_t i = 0; i < windows.size(); ++i) {
        windows[i].resizeToken =
            windows[i].window->RegisterListener<dubu::window::EventResize>(
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
    }
}
```
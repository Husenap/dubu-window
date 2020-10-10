#pragma once

namespace dubu::window {

struct EventResize {
	int32_t width;
	int32_t height;
};

struct EventContentScale {
	float scaleX;
	float scaleY;
};

struct EventKey {
	int32_t key;
	int32_t scancode;
	int32_t action;
	int     mods;
};
struct EventKeyPress {
	int32_t key;
	int32_t scancode;
	int32_t mods;
};
struct EventKeyRelease {
	int32_t key;
	int32_t scancode;
	int32_t mods;
};
struct EventKeyRepeat {
	int32_t key;
	int32_t scancode;
	int32_t mods;
};

struct EventChar {
	uint32_t codepoint;
};

struct EventCursorPos {
	double posX;
	double posY;
};
struct EventCursorEnter {};
struct EventCursorLeave {};

struct EventMouseButton {
	int32_t button;
	int32_t action;
	int32_t mods;
};
struct EventMouseButtonPress {
	int32_t button;
	int32_t mods;
};
struct EventMouseButtonRelease {
	int32_t button;
	int32_t mods;
};

struct EventScroll {
	double offsetX;
	double offsetY;
};

struct EventDroppedFile {
	std::filesystem::path file;
};

}  // namespace dubu::window
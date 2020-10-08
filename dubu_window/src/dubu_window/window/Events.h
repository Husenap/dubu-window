#pragma once

namespace dubu::window {

struct EventResize {
	int width;
	int height;
};

struct EventKey {
	int key;
	int scancode;
	int action;
	int mods;
};
struct EventKeyPress {
	int key;
	int scancode;
	int mods;
};
struct EventKeyRelease {
	int key;
	int scancode;
	int mods;
};
struct EventKeyRepeat {
	int key;
	int scancode;
	int mods;
};

struct EventDroppedFile {
	std::filesystem::path file;
};

struct EventContentScale {
	float sx;
	float sy;
};

}  // namespace dubu::window
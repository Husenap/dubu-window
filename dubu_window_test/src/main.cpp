#include <dubu_window/dubu_window.h>

#include <thread>

int main(){
	dubu::window::Window window;

	while(!window.ShouldClose()){
		window.PollEvents();

		std::this_thread::yield();
	}
}

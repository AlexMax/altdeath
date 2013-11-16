#include "SDL.h"

namespace input {

void poll() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYUP || event.type == SDL_QUIT) {
			// Yes, we're throwing an integer as an exception to quit.
			throw 0;
		}
	}
}

}

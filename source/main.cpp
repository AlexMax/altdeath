#include <cstdio>
#include <memory>
#include <stdexcept>

#include "SDL.h"
#include "GL/glew.h"

namespace renderer {

class Screen {
private:
	SDL_Window* window;
	SDL_GLContext glContext;
public:
	Screen();
	~Screen();
	void swap();
};

Screen::Screen() : window(NULL), glContext(NULL) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw new std::runtime_error(SDL_GetError());
	}

	this->window = SDL_CreateWindow("Altdeath Engine Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
		SDL_WINDOW_OPENGL);
	if (this->window == NULL) {
		throw new std::runtime_error(SDL_GetError());
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(this->window);
	if (glContext == NULL) {
		throw new std::runtime_error(SDL_GetError());
	}

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		throw new std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(glewError)));
	}
}

Screen::~Screen() {
	if (this->glContext != NULL) {
		SDL_GL_DeleteContext(glContext);
	}
	if (this->window != NULL) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void Screen::swap() {
	SDL_GL_SwapWindow(this->window);
}

}

int main(int argc, char** argv) {
	try {
		// Initialize a screen for us to target.
		std::unique_ptr<renderer::Screen> screen(new renderer::Screen);

		// Allocate a Vertex Array (why?)
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// Triangle data
		const GLfloat triangle[9] = {
			.0f, .75f, .0f,
			.75f, -.75f, .0f,
			-.75f, -.75f, .0f
		};

		// Pipe our vertex data into a VBO and hand it off to the video card
		GLuint vertexBufferID;
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

		for (;;) {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYUP || event.type == SDL_QUIT) {
					// Yes, we're throwing an integer as an exception to quit.				
					throw 0;
				}
			}

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// The actual draw call!
			// It's a triangle, start at vertex 0 and draw all three vertexes.
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisableVertexAttribArray(0);

			screen->swap();
		}
	}
	catch (const std::runtime_error &e) {
		fprintf(stderr, "Runtime exception: %s\n", e.what());
		return 1;
	}
	catch (int retval) {
		return retval;
	}

	// How did we get here?
	return 255;
}
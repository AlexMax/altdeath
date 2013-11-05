#include <cstdio>

#include "SDL.h"
#include "GL/glew.h"

int main(int argc, char** argv) {
	// SDL Initialization
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init() error: %s\n", SDL_GetError());
		return 1;
	}

	// SDL Window Initialization
	SDL_Window* window = SDL_CreateWindow("Altdeath Engine Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
		SDL_WINDOW_OPENGL);
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow() error: %s\n", SDL_GetError());

		SDL_Quit();
		return 1;
	}

	// OpenGL Context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		fprintf(stderr, "SDL_GL_CreateContext() error: %s\n", SDL_GetError());

		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "glewInit() error: %s\n", glewGetErrorString(glewError));

		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	if (!GLEW_VERSION_2_1) {
		fprintf(stderr, "Video Card must support OpenGL 2.1 or later\n");

		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYUP || event.type == SDL_QUIT) {
				SDL_GL_DeleteContext(glContext);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
		}

		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
		glEnd();

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return 0;
}

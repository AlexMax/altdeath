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
				SDL_GL_DeleteContext(glContext);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
		}

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// The actual draw call!
		// It's a triangle, start at vertex 0 and draw all three vertexes.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return 0;
}

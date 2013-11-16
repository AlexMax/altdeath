#include "SDL.h"
#include "opengl/gl_core_3_1.hpp"

namespace render {

class Screen {
private:
	Screen(const Screen&);
	Screen& operator=(const Screen&);

	SDL_Window* window;
	SDL_GLContext glContext;
public:
	Screen();
	~Screen();

	void swap();
};

class Shader {
private:
	GLuint ref;
public:
	Shader(const char* source, GLenum type);
	~Shader();
	GLuint getRef();
};

class Program {
private:
	GLuint ref;
public:
	Program();
	~Program();
	void attachShader(Shader &shader);
	void link();
	void use();
	GLuint getRef();
};

}
#include <stdexcept>

#include "render.hpp"

namespace render {

Screen::Screen() : window(NULL), glContext(NULL) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw new std::runtime_error(SDL_GetError());
	}

	// TODO: My eventual target is an Intel HD Graphics 2000, which supports 3.1.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	if (gl::sys::LoadFunctions() == false) {
		throw new std::runtime_error("OpenGL function loading failed.");
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

Shader::Shader(const char* source, GLenum type) {
	const GLchar* sourceArray[1] = { source };

	this->ref = gl::CreateShader(type);
	gl::ShaderSource(this->ref, 1, sourceArray, NULL);
	gl::CompileShader(this->ref);

	GLint success;
	gl::GetShaderiv(this->ref, gl::COMPILE_STATUS, &success);
	if (success == gl::FALSE_) {
		GLsizei infoLogLength;
		gl::GetShaderiv(this->ref, gl::INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		gl::GetShaderInfoLog(this->ref, infoLogLength, NULL, infoLog);

		throw std::runtime_error(infoLog);
	}
}

Shader::~Shader() {
	if (gl::IsShader(this->ref)) {
		gl::DeleteShader(this->ref);
	}
}

GLuint Shader::getRef() {
	return this->ref;
}


Program::Program() {
	this->ref = gl::CreateProgram();
}

Program::~Program() {
	if (gl::IsProgram(this->ref)) {
		gl::DeleteProgram(this->ref);
	}
}

void Program::attachShader(Shader &shader) {
	gl::AttachShader(this->ref, shader.getRef());
}

void Program::link() {
	gl::LinkProgram(this->ref);

	GLint success;
	gl::GetProgramiv(this->ref, gl::LINK_STATUS, &success);
	if (success == gl::FALSE_) {
		GLsizei infoLogLength;
		gl::GetProgramiv(this->ref, gl::INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		gl::GetShaderInfoLog(this->ref, infoLogLength, NULL, infoLog);

		throw std::runtime_error(infoLog);
	}
}

void Program::use() {
	gl::UseProgram(this->ref);
}

GLuint Program::getRef() {
	return this->ref;
}

}
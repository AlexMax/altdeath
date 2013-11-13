#include <cstdio>
#include <memory>
#include <stdexcept>

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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

	glewExperimental = GL_TRUE; // [AM] Fixes a glGenVertexArrays crash on OSX.
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

class Shader {
private:
	GLuint ref;
public:
	Shader(const char* source, GLenum type);
	~Shader();
	const GLuint getRef();
};

Shader::Shader(const char* source, GLenum type) {
	const GLchar* sourceArray[1] = { source };

	this->ref = glCreateShader(type);
	glShaderSource(this->ref, 1, sourceArray, NULL);
	glCompileShader(this->ref);

	GLint success;
	glGetShaderiv(this->ref, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLsizei infoLogLength;
		glGetShaderiv(this->ref, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(this->ref, infoLogLength, NULL, infoLog);

		throw std::runtime_error(infoLog);
	}
}

Shader::~Shader() {
	if (glIsShader(this->ref)) {
		glDeleteShader(this->ref);
	}
}

const GLuint Shader::getRef() {
	return this->ref;
}

class Program {
private:
	GLuint ref;
public:
	Program();
	~Program();
	void attachShader(Shader &shader);
	void link();
	void use();
	const GLuint getRef();
};

Program::Program() {
	this->ref = glCreateProgram();
}

Program::~Program() {
	if (glIsProgram(this->ref)) {
		glDeleteProgram(this->ref);
	}
}

void Program::attachShader(Shader &shader) {
	glAttachShader(this->ref, shader.getRef());
}

void Program::link() {
	glLinkProgram(this->ref);

	GLint success;
	glGetProgramiv(this->ref, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLsizei infoLogLength;
		glGetProgramiv(this->ref, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(this->ref, infoLogLength, NULL, infoLog);

		throw std::runtime_error(infoLog);
	}
}

void Program::use() {
	glUseProgram(this->ref);
}

const GLuint Program::getRef() {
	return this->ref;
}

}

int main(int argc, char** argv) {
	try {
		// Initialize a screen for us to target.
		std::unique_ptr<renderer::Screen> screen(new renderer::Screen);

		const char* vertex =
			"#version 140\n"
			"in vec3 a_position;"
			"in vec3 a_color;"
			"out vec3 v_color;"
			"uniform mat4 u_mvp;"
			"void main() {"
			"v_color = a_color;"
			"gl_Position = u_mvp * vec4(a_position, 1.f);"
			"}";
		renderer::Shader vs = renderer::Shader(vertex, GL_VERTEX_SHADER);

		const char* fragment =
			"#version 140\n"
			"in vec3 v_color;"
			"out vec4 fragcolor;"
			"void main() {"
			"fragcolor = vec4(v_color, 1);"
			"}";
		renderer::Shader fs = renderer::Shader(fragment, GL_FRAGMENT_SHADER);

		// Compile and link the Shader Program
		renderer::Program program;
		program.attachShader(vs);
		program.attachShader(fs);
		program.link();
		program.use();

		GLint a_position = glGetAttribLocation(program.getRef(), "a_position");
		glEnableVertexAttribArray(a_position);
		GLint a_color = glGetAttribLocation(program.getRef(), "a_color");
		glEnableVertexAttribArray(a_color);

		// Create a base Vertex Array Object
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

#define DELIM ,
#define LINE(x1,y1,x2,y2) \
	x1.0f DELIM y1.0f DELIM 128.0f DELIM \
	x2.0f DELIM y2.0f DELIM 128.0f DELIM \
	x1.0f DELIM y1.0f DELIM 0.0f DELIM \
	x1.0f DELIM y1.0f DELIM 0.0f DELIM \
	x2.0f DELIM y2.0f DELIM 128.0f DELIM \
	x2.0f DELIM y2.0f DELIM 0.0f
#define COLOR(r,g,b) \
	r.0f DELIM g.0f DELIM b.0f DELIM \
	r.0f DELIM g.0f DELIM b.0f DELIM \
	r.0f DELIM g.0f DELIM b.0f DELIM \
	r.0f DELIM g.0f DELIM b.0f DELIM \
	r.0f DELIM g.0f DELIM b.0f DELIM \
	r.0f DELIM g.0f DELIM b.0f

		// Triangle data: D_RUNNIN
		const GLfloat triangle[] = {
			// Main Room
			LINE(-224, 512, 228, 512),
			LINE(228, 512, 352, 192),
			LINE(352, 192, 224, 64),
			LINE(224, 64, 160, 64),
			LINE(160, 64, 160, 0),
			LINE(160, 0, 32, -64),
			LINE(32, -64, -32, -64),
			LINE(-32, -64, -96, 0),
			LINE(-96, 0, -96, 64),
			LINE(-96, 64, -160, 64),
			LINE(-160, 64, -288, 192),
			LINE(-288, 192, -224, 512),

			// Left Pillar
			LINE(-160, 256, -96, 256),
			LINE(-96, 256, -96, 192),
			LINE(-96, 192, -160, 192),
			LINE(-160, 192, -160, 256),

			// Right Pillar
			LINE(160, 256, 224, 256),
			LINE(224, 256, 224, 192),
			LINE(224, 192, 160, 192),
			LINE(160, 192, 160, 256),

			// Main Room Color
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),

			// Left Pillar Color
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),

			// Right Pillar Color
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0),
			COLOR(0, 1, 0)
		};
		size_t colors = sizeof(triangle) / 2;

		// Pipe our vertex data into a VBO and hand it off to the video card
		GLuint vertexBufferID;
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

		// Projection
		glm::mat4 projection = glm::perspective(90.0f, 16.0f / 10.0f, 0.1f, 1000.0f);

		// View
		glm::mat4 viewRotate = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 viewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -56.0f));
		glm::mat4 view = viewRotate * viewTranslate;

		// Model
		glm::mat4 model = glm::mat4(1.0f);
		
		// Combined
		glm::mat4 MVP = projection * view * model;

		// Send our MVP matrix to the vertex shader in our shader program.
		GLuint u_mvp = glGetUniformLocation(program.getRef(), "u_mvp");
		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &MVP[0][0]);

		for (;;) {
			input::poll();

			// Clear the screen.
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)colors);

			// The actual draw call!
			// It's a triangle, start at vertex 0 and draw all three
			// vertexes for all three triangles.
			glDrawArrays(GL_TRIANGLES, 0, 6 * 20);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

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

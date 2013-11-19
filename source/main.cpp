#include <cstdio>
#include <memory>
#include <stdexcept>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "input.hpp"
#include "render.hpp"
#include "sim.hpp"

int main(int argc, char** argv) {
	// We can't simply omit argc and argv, otherwise SDL_main gets confused.
	(void)argc; (void)argv;

	sim::Map map;
	map.load("namespace = \"Altdeath\";\nlinedef { foo = \"bar\"; baz = 9; }");

	try {
		// Initialize a screen for us to target.
		std::unique_ptr<render::Screen> screen(new render::Screen);

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
		render::Shader vs = render::Shader(vertex, gl::VERTEX_SHADER);

		const char* fragment =
			"#version 140\n"
			"in vec3 v_color;"
			"out vec4 fragcolor;"
			"void main() {"
			"fragcolor = vec4(v_color, 1);"
			"}";
		render::Shader fs = render::Shader(fragment, gl::FRAGMENT_SHADER);

		// Compile and link the Shader Program
		render::Program program;
		program.attachShader(vs);
		program.attachShader(fs);
		program.link();
		program.use();

		GLint a_position = gl::GetAttribLocation(program.getRef(), "a_position");
		gl::EnableVertexAttribArray(a_position);
		GLint a_color = gl::GetAttribLocation(program.getRef(), "a_color");
		gl::EnableVertexAttribArray(a_color);

		// Create a base Vertex Array Object
		GLuint vertexArrayID;
		gl::GenVertexArrays(1, &vertexArrayID);
		gl::BindVertexArray(vertexArrayID);

		// Enable backface culling
		gl::Enable(gl::CULL_FACE);
		gl::CullFace(gl::BACK);
		gl::FrontFace(gl::CW);

#define DELIM ,
#define LINE(x1,y1,x2,y2) \
	(GLfloat)x1 DELIM (GLfloat)y1 DELIM 128.0f DELIM \
	(GLfloat)x2 DELIM (GLfloat)y2 DELIM 128.0f DELIM \
	(GLfloat)x1 DELIM (GLfloat)y1 DELIM 0.0f DELIM \
	(GLfloat)x1 DELIM (GLfloat)y1 DELIM 0.0f DELIM \
	(GLfloat)x2 DELIM (GLfloat)y2 DELIM 128.0f DELIM \
	(GLfloat)x2 DELIM (GLfloat)y2 DELIM 0.0f
#define COLOR(r,g,b) \
	(GLfloat)r DELIM (GLfloat)g DELIM (GLfloat)b DELIM \
	(GLfloat)r DELIM (GLfloat)g DELIM (GLfloat)b DELIM \
	(GLfloat)r DELIM (GLfloat)g DELIM (GLfloat)b DELIM \
	(GLfloat)r DELIM (GLfloat)g DELIM (GLfloat)b DELIM \
	(GLfloat)r DELIM (GLfloat)g DELIM (GLfloat)b DELIM \
	(GLfloat)r DELIM(GLfloat)g DELIM(GLfloat)b

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
			LINE(-96, 256, -160, 256),
			LINE(-96, 192, -96, 256),
			LINE(-160, 192, -96, 192),
			LINE(-160, 256, -160, 192),

			// Right Pillar
			LINE(224, 256, 160, 256),
			LINE(224, 192, 224, 256),
			LINE(160, 192, 224, 192),
			LINE(160, 256, 160, 192),

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
		gl::GenBuffers(1, &vertexBufferID);
		gl::BindBuffer(gl::ARRAY_BUFFER, vertexBufferID);
		gl::BufferData(gl::ARRAY_BUFFER, sizeof(triangle), triangle, gl::STATIC_DRAW);

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
		GLuint u_mvp = gl::GetUniformLocation(program.getRef(), "u_mvp");
		gl::UniformMatrix4fv(u_mvp, 1, gl::FALSE_, &MVP[0][0]);

		for (;;) {
			input::poll();

			// Clear the screen.
			gl::ClearColor(0.0, 0.0, 0.0, 1.0);
			gl::Clear(gl::COLOR_BUFFER_BIT);

			gl::EnableVertexAttribArray(0);
			gl::EnableVertexAttribArray(1);

			gl::BindBuffer(gl::ARRAY_BUFFER, vertexBufferID);
			gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, 0);
			gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, 0, (GLvoid*)colors);

			// The actual draw call!
			// It's a triangle, start at vertex 0 and draw all three
			// vertexes for all three triangles.
			gl::DrawArrays(gl::TRIANGLES, 0, 6 * 20);

			gl::DisableVertexAttribArray(0);
			gl::DisableVertexAttribArray(1);

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

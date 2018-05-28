#include "../Classes/Renderer/Core.h"
#include <iostream>

class simple_triangle
	: public Core
{
public:
	simple_triangle()
		: Core(800, 600, "OpenGL")
	{}

	virtual void Start() override 
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"void main()\n"
			"{\n"
			"	vec4 vertices[3] = vec4[3]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.0, 0.5, 0.0, 1.0)\n"
			"	);\n"
			"	gl_Position = vertices[gl_VertexID];\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0, 0.0, 0.0, 0.0);\n"
			"}"
		};

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
		glCompileShader(vertex_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
		glCompileShader(fragment_shader);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
	
	virtual void Update() override
	{
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
	}

private:
	GLuint shader_program;
};

CORE_MAIN(simple_triangle)
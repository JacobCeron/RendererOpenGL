#include "../Classes/Renderer/Core.h"

#include <iostream>

class atomic_counter_variable
	: public Core
{
public:
	atomic_counter_variable()
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
			"layout(binding = 0) uniform atomic_uint counter;\n"
			"layout(location = 0) uniform uint time;\n"
			"void main()\n"
			"{\n"
			"	uint count = atomicCounterIncrement(counter);\n"
			"	if (count < time)\n"
			"		FragColor = vec4(1.0);\n"
			"	else\n"
			"		FragColor = vec4(0.0);\n"
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

		glCreateBuffers(1, &atomic_counter);
		glNamedBufferStorage(atomic_counter, sizeof(GLuint), nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter);
	}

	virtual void Update() override
	{
		GLuint* data = (GLuint*)glMapNamedBufferRange(atomic_counter, 0, sizeof(GLuint), GL_MAP_WRITE_BIT);
		*data = 0;
		glUnmapNamedBuffer(atomic_counter);

		static float time{ 0.0f };
		time += Time::deltaTime * 500.0f;

		glUseProgram(shader_program);
		glUniform1ui(0, static_cast<GLuint>(time));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &atomic_counter);
	}

private:
	GLuint shader_program;
	GLuint atomic_counter;
};

#if 0
CORE_MAIN(atomic_counter_variable)
#endif
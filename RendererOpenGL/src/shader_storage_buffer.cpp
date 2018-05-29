#include "../Classes/Renderer/Core.h"

class shader_storage_buffer
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"layout(std430, binding = 0) buffer storage\n"
			"{\n"
			"	vec4 vPos[];\n"
			"};\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vPos[gl_VertexID];\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(0.0, 1.0, 1.0, 0.0);\n"
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

		vec4 vertices[]
		{
			vec4(-0.5f, -0.5f, 0.0f, 1.0f), vec4(0.5f, -0.5f, 0.0f, 1.0f), vec4(0.0f, 0.5f, 0.0f, 1.0f)
		};

		glCreateBuffers(1, &storage_buffer);
		glNamedBufferStorage(storage_buffer, sizeof(vertices), vertices, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storage_buffer);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time -= Time::deltaTime;

		void* ptr = glMapNamedBufferRange(storage_buffer, 0, sizeof(float), GL_MAP_WRITE_BIT);
		memcpy(ptr, &time, sizeof(float));
		glUnmapNamedBuffer(storage_buffer);

		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &storage_buffer);
	}

private:
	GLuint shader_program;
	GLuint storage_buffer;
};

#if 0
CORE_MAIN(shader_storage_buffer)
#endif
#include "../../Classes/Renderer/Core.h"

class vertex_array_data
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"
			"layout(location = 1) in vec4 vColor;\n"
			"out VS_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}vs_out;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vPos;\n"
			"	vs_out.color = vColor;\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"in VS_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}fs_in;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = fs_in.color;\n"
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

		struct vertices
		{
			vec2 pos[3]
			{
				vec2(-0.5f), vec2(0.5f, -0.5f), vec2(0.0f, 0.5f)
			};

			vec3 color[3]
			{
				vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)
			};
		};

		vertices v;

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(vertices), &v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, offsetof(vertices, pos));
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));
		glVertexArrayAttribFormat(vertex_array, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertices, color));
		glEnableVertexArrayAttrib(vertex_array, 1);
		glVertexArrayAttribBinding(vertex_array, 1, 1);
		glVertexArrayVertexBuffer(vertex_array, 1, vertex_buffer, 0, sizeof(vec3));

		Window::current->setColor(vec4(0.5f));
	}

	virtual void Update() override
	{
		glUseProgram(shader_program);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteBuffers(1, &vertex_buffer);
	}

private:
	GLuint shader_program;
	GLuint vertex_array;
	GLuint vertex_buffer;
};

#if 0
CORE_MAIN(vertex_array_data)
#endif
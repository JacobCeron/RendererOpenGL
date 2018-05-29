#include "../Classes/Renderer/Core.h"

class simple_texture
	: public Core
{
public:
	simple_texture()
		: Core(800, 600, "OpenGL")
	{}

	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"void main()\n"
			"{\n"
			"	vec4 vertices[4] = vec4[4]\n"
			"	(\n"
			"		vec4(-1.0, -1.0, 0.0, 1.0),\n"
			"		vec4(1.0, -1.0, 0.0, 1.0),\n"
			"		vec4(-1.0, 1.0, 0.0, 1.0),\n"
			"		vec4(1.0, 1.0, 0.0, 1.0)\n"
			"	);\n"
			"	gl_Position = vertices[gl_VertexID];\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"uniform sampler2D s;\n"
			"void main()\n"
			"{\n"
			"	FragColor = texelFetch(s, ivec2(gl_FragCoord.xy / 35.0), 0);\n"
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

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA32F, 800, 600);

		vec4 *data = new vec4[800 * 600];
		generate_texture(data, 800, 600);

		glTextureSubImage2D(texture, 0, 0, 0, 800, 600, GL_RGBA, GL_FLOAT, data);
		glBindTextureUnit(0, texture);
	}

	virtual void Update() override
	{
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteTextures(1, &texture);
	}

	void generate_texture(vec4* data, size_t w, size_t h)
	{
		for (size_t i{ 0 }; i < h; i++)
		{
			for (size_t j{ 0 }; j < w; j++)
			{
				if (j % 2 == 0 && i % 2 == 0)
					data[j + w * i] = vec4(1.0f);
				else
					data[j + w * i] = vec4(0.0f);
			}
		}
	}

private:
	GLuint shader_program;
	GLuint texture;
};

CORE_MAIN(simple_texture)
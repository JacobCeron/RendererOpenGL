#include "../Classes/Renderer/Core.h"

class multisample_texture
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"
			"layout(location = 1) in vec2 vUV;\n"
			"out VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}vs_out;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vPos;\n"
			"	vs_out.uv = vUV;\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"in VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}fs_in;\n"
			"out vec4 FragColor;\n"
			"layout(binding = 0) uniform sampler2D s;\n"
			"layout(binding = 1) uniform sampler2D t;\n"
			"void main()\n"
			"{\n"
			"	FragColor = mix(texture(s, fs_in.uv), texture(t, fs_in.uv), 0.5);\n"
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
			vec2 pos[4]
			{
				vec2(-1.0f, -1.0f), vec2(1.0f, -1.0f), vec2(-1.0f, 1.0f), vec2(1.0f, 1.0f)
			};
			vec2 uv[4]
			{
				vec2(0.0f), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f)
			};
		};

		vertices v;

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), &v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, offsetof(vertices, pos));
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayAttribFormat(vertex_array, 1, 2, GL_FLOAT, GL_FALSE, offsetof(vertices, uv));
		glEnableVertexArrayAttrib(vertex_array, 1);
		glVertexArrayAttribBinding(vertex_array, 1, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glCreateTextures(GL_TEXTURE_2D, 2, texture);
		glTextureStorage2D(texture[0], 1, GL_RGBA32F, 25, 25);

		vec4* data = new vec4[25 * 25];
		generate_texture(data, 25, 25);

		glTextureSubImage2D(texture[0], 0, 0, 0, 25, 25, GL_RGBA, GL_FLOAT, data);
		glTextureParameteri(texture[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTextureUnit(0, texture[0]);

		glTextureStorage2D(texture[1], 1, GL_RGBA32F, 25, 25);
		
		generate_texture2(data, 25, 25);

		glTextureSubImage2D(texture[1], 0, 0, 0, 25, 25, GL_RGBA, GL_FLOAT, data);
		glTextureParameteri(texture[1], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTextureUnit(1, texture[1]);
	}

	virtual void Update() override
	{
		glUseProgram(shader_program);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteTextures(2, texture);
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

	void generate_texture2(vec4* data, size_t w, size_t h)
	{
		for (size_t i{ 0 }; i < h; i++)
		{
			for (size_t j{ 0 }; j < w; j++)
			{
				if (j % 2 == 0 && i % 2 == 0)
					data[j + w * i] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
				else
					data[j + w * i] = vec4(1.0f, 1.0f, 0.0f, 0.0f);
			}
		}
	}

private:
	GLuint shader_program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint texture[2];
	GLuint sampler;
};

#if 0
CORE_MAIN(multisample_texture)
#endif
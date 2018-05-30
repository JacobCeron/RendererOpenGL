#include "../Classes/Renderer/Core.h"

class texture_wrap
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"out VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}vs_out;\n"
			"void main()\n"
			"{\n"
			"	vec4 vertices[4] = vec4[4]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(-0.5, 0.5, 0.0, 1.0),\n"
			"		vec4(0.5, 0.5, 0.0, 1.0)\n"
			"	);\n"

			"	vec4 pos = vertices[gl_VertexID];\n"
			"	vs_out.uv = pos.xy + vec2(1.0);\n"
			"	gl_Position = pos;\n"
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
			"void main()\n"
			"{\n"
			"	FragColor = mix(vec4(0.0, 0.1, 0.3, 0.0), texture(s, fs_in.uv), 0.5);\n"
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
		glTextureStorage2D(texture, 1, GL_RGBA32F, 255, 255);

		vec4* data = new vec4[255 * 255];
		generate_texture(data, 255, 255);

		glTextureSubImage2D(texture, 0, 0, 0, 255, 255, GL_RGBA, GL_FLOAT, data);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		//glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		vec4 border_color(1.0f, 1.0f, 0.0f, 1.0f);
		glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, &border_color[0]);
		//glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

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
				data[j + w * i].r = (float)((i & j) & 0xFF) / 255.0f;
				data[j + w * i].g = (float)((i & j) & 0xFF) / 255.0f;
				data[j + w * i].b = (float)((i & j) & 0xFF) / 255.0f;
				data[j + w * i].a = 1.0f;
			}
		}
	}

private:
	GLuint shader_program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint texture;
};

#if 0
CORE_MAIN(texture_wrap)
#endif
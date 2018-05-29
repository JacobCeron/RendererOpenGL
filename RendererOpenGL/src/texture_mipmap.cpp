#include "../Classes/Renderer/Core.h"
#include "../Classes/Maths/maths.h"

class texture_mipmap
	: public Core
{
public:
	texture_mipmap()
		: Core(800, 600, "OpenGL")
	{}

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

			"layout(location = 0) uniform float time;\n"

			"layout(std140, binding = 0) uniform TRNASFORM\n"
			"{\n"
			"	mat4 M;\n"
			"	mat4 V;\n"
			"	mat4 P;\n"
			"}trans;\n"

			"void main()\n"
			"{\n"
			"	gl_Position = trans.P * trans.M * vPos;\n"
			"	vs_out.uv = vec2(vUV.x + time, vUV.y);\n"
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
			"	FragColor = texture(s, fs_in.uv);\n"
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
				vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f, 0.5f)
			};
			vec2 uv[4]
			{
				vec2(0.0f), vec2(30.0f, 0.0f), vec2(0.0f, 2.0f), vec2(30.0f, 2.0f)
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

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
		
		mat4 V;
		V = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 P;
		P = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		mat4 *trans = (mat4*)glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		trans[0] = V;
		trans[1] = P;
		glUnmapNamedBuffer(uniform_buffer);

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 8, GL_RGBA32F, 500, 500);

		vec4* data = new vec4[500 * 500];
		generate_texture(data, 500, 500);

		glTextureSubImage2D(texture, 0, 0, 0, 500, 500, GL_RGBA, GL_FLOAT, data);
		glGenerateTextureMipmap(texture);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTextureUnit(0, texture);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		glUseProgram(shader_program);
		glUniform1f(0, time);

		for (size_t i{ 0 }; i < 8; i++)
		{
			mat4 M;
			M = rotate(M, radians(45.0f * (i + 1 * 2)), vec3(0.0f, 0.0f, 1.0f));
			M = translate(M, vec3(-1.0f, 0.0f, -3.0f));
			M = rotate(M, radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
			M = scale(M, vec3(30.0f, 2.0f, 1.0f));
			void* ptr = glMapNamedBufferRange(uniform_buffer, 0, sizeof(M), GL_MAP_WRITE_BIT);
			memcpy(ptr, &M[0][0], sizeof(M));
			glUnmapNamedBuffer(uniform_buffer);
			glBindVertexArray(vertex_array);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &uniform_buffer);
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
	GLuint uniform_buffer;
	GLuint texture;
};

#if 1
CORE_MAIN(texture_mipmap)
#endif
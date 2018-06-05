#include "../../Classes/Renderer/Core.h"
#include "../../Classes/Renderer/stb_image.h"

#include <iostream>

class plane_deformation
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"
			"layout(location = 1) in vec2 vUV;\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

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

		const char* fs_source
		{
			"#version 450 core\n"

			"in VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}fs_in;\n"
			
			"out vec4 FragColor;\n"

			"layout(binding = 0) uniform sampler2D s;\n"
			"layout(location = 1) uniform float time;\n"

			"void main()\n"
			"{\n"
			"	float r = 1.0 / distance(vec2(0.5, 0.5), fs_in.uv) * 2.0;\n"
			"	vec2 pct = vec2(r);\n"
			"	FragColor = texture(s, 0.3 / sin( 0.5 / pct + time * 0.05));\n"
			"}"
		};

		vs_program = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vs_source);
		fs_program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_source);
		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program);

		vec2 v[]
		{
			vec2(-1.0f, -1.0f),		vec2(0.0f, 0.0f),
			vec2( 1.0f, -1.0f),		vec2(1.0f, 0.0f),
			vec2(-1.0f,  1.0f),		vec2(0.0f, 1.0f),
			vec2( 1.0f,  1.0f),		vec2(1.0f, 1.0f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);

		glVertexArrayAttribFormat(vertex_array, 1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2));
		glEnableVertexArrayAttrib(vertex_array, 1);
		glVertexArrayAttribBinding(vertex_array, 1, 0);

		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2) * 2);

		int width;
		int height;
		int channel;
		unsigned char* data = stbi_load("C:/Users/UserHp/Desktop/test_image_iq.jpg", &width, &height, &channel, 0);
		
		std::cout << width << " " << height << " " << channel << std::endl;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGB8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(0, texture);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		glBindProgramPipeline(program_pipeline);
		glProgramUniform1f(fs_program, 1, time);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteTextures(1, &texture);
	}

private:
	GLuint vs_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint vertex_array;
	GLuint texture;
};

#if 0
CORE_MAIN(plane_deformation)
#endif
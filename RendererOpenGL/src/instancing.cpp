#include "../Classes/Renderer/Core.h"
#include "../Classes/Renderer/Input.h"

#include <iostream>


class instancing
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}vs_out;\n"

			"layout(std140, binding = 0) uniform TRANSFORM\n"
			"{\n"
			"	mat4 Model;\n"
			"	mat4 View;\n"
			"	mat4 Projection;\n"
			"}transform;\n"

			"layout(location = 0) uniform int bits;\n"

			"void main()\n"
			"{\n"
			"	vec4 pos = transform.Model * vPos;\n"

			"	int x = bitfieldExtract(gl_InstanceID, 0, bits);\n"
			"	int y = bitfieldExtract(gl_InstanceID, bits, bits);\n"

			"	pos += vec4(x * 0.5, y * 0.5, 0.0, 0.0);\n"

			"	gl_Position = transform.Projection * transform.View * pos;\n"
			"	vs_out.color = vPos + 0.5;\n"
			"}"
		};

		const char* fs_source
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

		vs_program = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vs_source);
		if (!Get_Program_Status(vs_program, GL_LINK_STATUS))
			Get_Link_Info(vs_program);

		fs_program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_source);
		if (!Get_Program_Status(fs_program, GL_LINK_STATUS))
			Get_Link_Info(fs_program);

		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program);

		vec3 v[]
		{
			vec3(vec2(-0.5f), 0.5f), vec3(0.5f, -0.5f, 0.5f), vec3(0.5f), vec3(-0.5f, vec2(0.5f)),
			vec3(-0.5f), vec3(0.5f, vec2(-0.5f)), vec3(vec2(0.5f), -0.5f), vec3(-0.5f, 0.5f, -0.5f)
		};

		GLubyte i[]
		{
			0, 1, 3,
			2, 7, 6,
			4, 5, 0,
			1, 5, 2,
			6, 7, 4,
			0, 7, 3
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), &v, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &index_buffer);
		glNamedBufferStorage(index_buffer, sizeof(i), &i, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec3));

		mat4 View;
		View = lookAt(vec3(0.0f, 0.0f, 12.0f), vec3(0.0f, -3.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(60.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		
		mat4* transforms = (mat4*)glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		transforms[0] = View;
		transforms[1] = Projection;
		glUnmapNamedBuffer(uniform_buffer);

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(10);
		glPrimitiveRestartIndex(13);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4 Model;
		Model = scale(Model, vec3(0.25f));
		Model = translate(Model, vec3(0.0f, -6.75f, -12.0f));
		Model = rotate(Model, time, vec3(1.0f));

		void* ptr = glMapNamedBufferRange(uniform_buffer, 0, sizeof(Model), GL_MAP_WRITE_BIT);
		memcpy(ptr, &Model, sizeof(Model));
		glUnmapNamedBuffer(uniform_buffer);

		glBindProgramPipeline(program_pipeline);

		glProgramUniform1i(vs_program, 0, 6);

		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_BYTE, 0, 600'000);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);
		glDeleteBuffers(1, &uniform_buffer);
		glDeleteVertexArrays(1, &vertex_array);
	}

	GLint Get_Shader_Status(GLuint shader, GLenum type)
	{
		GLint status;
		glGetShaderiv(shader, type, &status);
		return status;
	}

	GLint Get_Program_Status(GLuint program, GLenum type)
	{
		GLint status;
		glGetProgramiv(program, type, &status);
		return status;
	}

	void Get_Compile_Info(GLuint shader)
	{
		GLint info_size{ Get_Shader_Status(shader, GL_INFO_LOG_LENGTH) };
		char* info_log = new char[info_size];
		glGetShaderInfoLog(shader, info_size, nullptr, info_log);
		std::cout << info_log << std::endl;
	}

	void Get_Link_Info(GLuint program)
	{
		GLint info_size{ Get_Program_Status(program, GL_INFO_LOG_LENGTH) };
		char* info_log = new char[info_size];
		glGetProgramInfoLog(program, info_size, nullptr, info_log);
		std::cout << info_log << std::endl;
	}

private:
	GLuint vs_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint uniform_buffer;
	GLuint vertex_array;
};

#if 0
CORE_MAIN(instancing)
#endif
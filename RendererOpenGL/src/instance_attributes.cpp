#include "../Classes/Renderer/Core.h"

class instance_attributes
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"
			"layout(location = 1) in vec4 instance_color;\n"
			"layout(location = 2) in vec4 instance_position;\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}vs_out;\n"

			"layout(location = 0) uniform mat4 M;\n"
			"layout(location = 2) uniform mat4 P;\n"

			"void main()\n"
			"{\n"
			"	gl_Position = P * M * (vPos + instance_position);\n"
			"	vs_out.color = instance_color;\n"
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
		fs_program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_source);
		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program);

		vec2 v[]
		{
			vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f, 0.5f)
		};

		vec3 c[]
		{
			vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)
		};

		vec2 p[]
		{
			vec2(-1.0f), vec2(1.0f, -1.0f), vec2(1.0f), vec2(-1.0f, 1.0f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v) + sizeof(c) + sizeof(p), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vertex_buffer, 0, sizeof(v), v);
		glNamedBufferSubData(vertex_buffer, sizeof(v), sizeof(c), c);
		glNamedBufferSubData(vertex_buffer, sizeof(v) + sizeof(c), sizeof(p), p);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glVertexArrayAttribFormat(vertex_array, 1, 3, GL_FLOAT, GL_FALSE, sizeof(v));
		glEnableVertexArrayAttrib(vertex_array, 1);
		glVertexArrayAttribBinding(vertex_array, 1, 1);
		glVertexArrayVertexBuffer(vertex_array, 1, vertex_buffer, 0, sizeof(vec3));
		
		glVertexArrayAttribFormat(vertex_array, 2, 2, GL_FLOAT, GL_FALSE, sizeof(v) + sizeof(c));
		glEnableVertexArrayAttrib(vertex_array, 2);
		glVertexArrayAttribBinding(vertex_array, 2, 2);
		glVertexArrayVertexBuffer(vertex_array, 2, vertex_buffer, 0, sizeof(vec2));

		glVertexArrayBindingDivisor(vertex_array, 1, 1);
		glVertexArrayBindingDivisor(vertex_array, 2, 1);
	}

	virtual void Update() override
	{
		glBindProgramPipeline(program_pipeline);

		mat4 M;
		M = translate(M, vec3(0.0f, 0.0f, -3.0f));
		mat4 P;
		P = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		glProgramUniformMatrix4fv(vs_program, 0, 1, GL_FALSE, &M[0][0]);
		glProgramUniformMatrix4fv(vs_program, 2, 1, GL_FALSE, &P[0][0]);

		glBindVertexArray(vertex_array);
		glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, 4, 0);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteVertexArrays(1, &vertex_array);
	}
private:
	GLuint vs_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint vertex_array;
};

#if 0
CORE_MAIN(instance_attributes)
#endif
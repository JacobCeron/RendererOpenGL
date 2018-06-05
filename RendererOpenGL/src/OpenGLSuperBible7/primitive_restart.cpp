#include "../../Classes/Renderer/Core.h"

class primitive_restart
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

			"layout(std140, binding = 0) uniform TRANSFORMS\n"
			"{\n"
			"	mat4 Model;\n"
			"	mat4 View;\n"
			"	mat4 Projection;\n"
			"}trans;\n"

			"void main()\n"
			"{\n"
			"	gl_Position = trans.Projection * trans.View * trans.Model * vPos;\n"
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
		fs_program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_source);
		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program);

		vec3 v[]
		{
			vec3(-3.0f, 1.0f, 0.0f),	vec3(-2.0f, -1.0f, 0.0f),	vec3(-1.0f, 1.0f, 0.0f),	vec3(0.0f, -1.0f, 0.0f),
			vec3( 1.0f, 1.0f, 0.0f),	vec3( 2.0f, -1.0f, 0.0f),	vec3( 3.0f, 1.0f, 0.0f)
		};

		GLubyte i[]
		{
			0, 1, 2,
			3, 4, 5,
			6, 4
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &index_buffer);
		glNamedBufferStorage(index_buffer, sizeof(i), i, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec3));

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

		mat4 View;
		View = lookAt(vec3(vec2(0.0f), 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		glNamedBufferSubData(uniform_buffer, sizeof(mat4), sizeof(mat4), &View);
		glNamedBufferSubData(uniform_buffer, sizeof(mat4) * 2, sizeof(mat4), &Projection);

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0x03);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4 Model;
		Model = translate(Model, vec3(vec2(0.0f), -3.0f));
		glNamedBufferSubData(uniform_buffer, 0, sizeof(Model), &Model);

		glBindProgramPipeline(program_pipeline);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 0));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &vertex_array);
		glDeleteBuffers(1, &index_buffer);
		glDeleteBuffers(1, &uniform_buffer);
	}

private:
	GLuint vs_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint vertex_array;
	GLuint uniform_buffer;
};

#if 0
CORE_MAIN(primitive_restart)
#endif
#include "../Classes/Renderer/Core.h"

class simple_indexed_drawing_command
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
			"	vec4 gl_Position;"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec4 color;"
			"}vs_out;\n"

			"layout(std430, binding = 0) buffer TRANSFORMS\n"
			"{\n"
			"	mat4 Model;\n"
			"	mat4 View;\n"
			"	mat4 Projection;\n"
			"};\n"
			
			"void main()\n"
			"{\n"
			"	gl_Position = Projection * View * Model * vPos;\n"
			"	vs_out.color = vPos + 0.5;\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"

			"in VS_OUT\n"
			"{\n"
			"	vec4 color;"
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
			vec3(vec2(-0.5f), 0.5f), vec3(0.5f, -0.5f, 0.5f), vec3(0.5f), vec3(-0.5f, vec2(0.5f)),
			vec3(-0.5f), vec3(0.5f, vec2(-0.5f)), vec3(vec2(0.5f), -0.5f), vec3(-0.5f, 0.5f, -0.5f)
		};

		GLubyte i[]
		{
			0, 1, 3,
			1, 2, 3,
			3, 2, 7,
			2, 6, 7,
			4, 5, 7,
			5, 6, 7,
			0, 1, 4,
			1, 5, 4,
			1, 5, 2,
			5, 6, 2,
			4, 0, 7,
			0, 3, 7
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), &v, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &element_buffer);
		glNamedBufferStorage(element_buffer, sizeof(i), i, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec3));

		glCreateBuffers(1, &storage_buffer);
		glNamedBufferStorage(storage_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storage_buffer);

		mat4 View;
		View = lookAt(vec3(vec2(0.0f), 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		
		mat4 *transforms = (mat4*)glMapNamedBufferRange(storage_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		transforms[0] = View;
		transforms[1] = Projection;
		glUnmapNamedBuffer(storage_buffer);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4 Model;
		Model = translate(Model, vec3(vec2(0.0f), -3.0f));
		Model = rotate(Model, time, vec3(vec2(1.0f), 0.0f));

		void* ptr = glMapNamedBufferRange(storage_buffer, 0, sizeof(Model), GL_MAP_WRITE_BIT);
		memcpy(ptr, &Model, sizeof(Model));
		glUnmapNamedBuffer(storage_buffer);

		glBindProgramPipeline(program_pipeline);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &element_buffer);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteBuffers(1, &storage_buffer);
	}

private:
	GLuint vs_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLuint vertex_array;
	GLuint storage_buffer;
};

#if 1
CORE_MAIN(simple_indexed_drawing_command)
#endif
#include "../Classes/Renderer/Core.h"

class clipping
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
			"	vec4 gl_ClipDistance[];\n"
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
			"}transform;\n"

			"vec4 clip_plane1 = vec4(1.0, -1.0, 0.0, 0.0);\n"
			"vec4 clip_plane2 = vec4(0.0, -0.2, 0.0, 0.0);\n"

			"void main()\n"
			"{\n"
			"	gl_ClipDistance[0] = dot(vPos, clip_plane1);\n"
			"	gl_ClipDistance[1] = dot(vPos, clip_plane2);\n"
			"	gl_Position = transform.Projection * transform.View * transform.Model * vPos;\n"
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
			vec3(-0.5f, -0.5f,	0.5f),	vec3(0.5f, -0.5f,  0.5f),	vec3(0.5f, 0.5f,  0.5f),	vec3(-0.5f, 0.5f,  0.5f),
			vec3(-0.5f, -0.5f, -0.5f),	vec3(0.5f, -0.5f, -0.5f),	vec3(0.5f, 0.5f, -0.5f),	vec3(-0.5f, 0.5f, -0.5f)
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
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &index_buffer);
		glNamedBufferStorage(index_buffer, sizeof(i), i, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec3));

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

		mat4 View;
		View = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);

		mat4* transforms = static_cast<mat4*>(glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT));
		transforms[0] = View;
		transforms[1] = Projection;
		glUnmapNamedBuffer(uniform_buffer);

		glEnable(GL_CLIP_DISTANCE0);
		glEnable(GL_CLIP_DISTANCE1);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4* Model = static_cast<mat4*>(glMapNamedBufferRange(uniform_buffer, 0, sizeof(mat4), GL_MAP_WRITE_BIT));
		Model[0] = translate(mat4(), vec3(0.0f, 0.0f, -3.0f));
		Model[0] = rotate(Model[0], time, vec3(0, 1, 1));
		glUnmapNamedBuffer(uniform_buffer);

		glBindProgramPipeline(program_pipeline);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(sizeof(GLuint) * 0), 1, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

#if 1
CORE_MAIN(clipping)
#endif

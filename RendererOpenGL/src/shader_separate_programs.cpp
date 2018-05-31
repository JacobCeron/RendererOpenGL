#include "../Classes/Renderer/Core.h"
#include "../Classes/Renderer/Input.h"

class shader_separate_program
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source1
		{
			"#version 450 core\n"

			"out gl_PerVertex\n"		// This must be appear in every shader source (that use gl_Position) to match correctly every single shader stage
			"{\n"	
			"	vec4 gl_Position;\n"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}vs_out;\n"

			"void main()\n"
			"{\n"
			"	vec4 vertex[3] = vec4[3]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.5, 1.0),"
			"		vec4(0.5, -0.5, 0.5, 1.0),"
			"		vec4(0.0, 0.5, 0.5, 1.0)"
			"	);\n"
			"	gl_Position = vertex[gl_VertexID];\n"
			"	vs_out.color = vec4(1.0, 1.0, 0.0, 0.0);\n"
			"}"
		};

		const char* vs_source2
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

			"void main()\n"
			"{\n"
			"	gl_Position = vPos;\n"
			"	vs_out.color = vPos + 0.5;\n"
			"}"
		};

		const char* fs_source1
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

		const char* fs_source2
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(0.4, 0.1, 1.0, 0.0);\n"
			"}"
		};

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vs_source1, nullptr);
		glCompileShader(vs);

		vs_program1 = glCreateProgram();
		glAttachShader(vs_program1, vs);

		glProgramParameteri(vs_program1, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(vs_program1);

		glShaderSource(vs, 1, &vs_source2, nullptr);
		glCompileShader(vs);

		vs_program2 = glCreateProgram();
		glAttachShader(vs_program2, vs);

		glProgramParameteri(vs_program2, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(vs_program2);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source1, nullptr);
		glCompileShader(fs);

		fs_program1 = glCreateProgram();
		glAttachShader(fs_program1, fs);

		glProgramParameteri(fs_program1, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(fs_program1);

		glShaderSource(fs, 1, &fs_source2, nullptr);
		glCompileShader(fs);

		fs_program2 = glCreateProgram();
		glAttachShader(fs_program2, fs);

		glProgramParameteri(fs_program2, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(fs_program2);

		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program2);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program1);

		glDeleteShader(vs);
		glDeleteShader(fs);

		vec2 pos[]{ vec2(-0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f) };

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(pos), pos, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		Window::current->setColor(vec4(0.2f));
	}

	virtual void Update() override
	{
		if (Input::getKeyDown(KeyCode::W))
			glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program1);
		if (Input::getKeyDown(KeyCode::S))
			glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program2);

		if (Input::getKeyDown(KeyCode::A))
			glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program1);
		if (Input::getKeyDown(KeyCode::D))
			glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program2);

		glBindProgramPipeline(program_pipeline);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program1);
		glDeleteProgram(vs_program2);
		glDeleteProgram(fs_program1);
		glDeleteProgram(fs_program2);
		glDeleteProgramPipelines(1, &program_pipeline);
	}

private:
	GLuint vs_program1;
	GLuint vs_program2;
	GLuint fs_program1;
	GLuint fs_program2;
	GLuint program_pipeline;

	GLuint vertex_array;
	GLuint vertex_buffer;
};

#if 0
CORE_MAIN(shader_separate_program)
#endif
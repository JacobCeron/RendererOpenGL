#include "../Classes/Renderer/Core.h"

class tessellation_lines
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"
			
			"void main()\n"
			"{\n"
			"	gl_Position = vPos;\n"
			"}"
		};

		const char* tsc_source
		{
			"#version 450 core\n"
			"layout(vertices = 4) out;\n"
			
			"void main()\n"
			"{\n"
			"	if (gl_InvocationID == 0)\n"
			"	{\n"
			"		gl_TessLevelOuter[0] = 6.0;\n"
			"		gl_TessLevelOuter[1] = 60.0;\n"
			"	}\n"

			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"}"
		};

		const char* tse_source
		{
			"#version 450 core\n"
			"layout(isolines) in;\n"

			"layout(location = 0) uniform float u_time;\n"
			
			"void main()\n"
			"{\n"
			"	float r = gl_TessCoord.x / gl_TessLevelOuter[0] + gl_TessCoord.y;\n"
			"	float t = gl_TessCoord.x * 2.0 * 3.14159;\n"
			"	gl_Position = vec4(r*cos(t + u_time), r*sin(t + u_time), 0.0, 1.0);\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"
			
			"out vec4 FragColor;\n"
			
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0, 0.0, 0.0, 0.0);\n"
			"}"
		};

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vs_source, nullptr);
		glCompileShader(vs);

		GLuint tsc = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tsc, 1, &tsc_source, nullptr);
		glCompileShader(tsc);

		GLuint tse = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tse, 1, &tse_source, nullptr);
		glCompileShader(tse);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source, nullptr);
		glCompileShader(fs);

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, tsc);
		glAttachShader(program, tse);
		glAttachShader(program, fs);
		glLinkProgram(program);

		glDeleteShader(vs);
		glDeleteShader(tsc);
		glDeleteShader(tse);
		glDeleteShader(fs);

		vec2 v[]
		{
			vec2(-0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_DYNAMIC_STORAGE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glPatchParameteri(GL_PATCH_VERTICES, 4);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		glUseProgram(program);
		glUniform1f(0, time * 6.0f);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_PATCHES, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &vertex_array);
	}

private:
	GLuint program;
	GLuint vertex_buffer;
	GLuint vertex_array;
};

#if 1
CORE_MAIN(tessellation_lines)
#endif
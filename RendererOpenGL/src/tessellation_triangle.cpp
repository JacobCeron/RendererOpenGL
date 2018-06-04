#include "../Classes/Renderer/Core.h"

#include <iostream>

class tessellation_triangle
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
			"layout(vertices = 3) out;\n"
			
			"void main()\n"
			"{\n"
			"	if (gl_InvocationID == 0)\n"
			"	{\n"
			"		gl_TessLevelInner[0] = 3.0;\n"
			"		gl_TessLevelOuter[0] = 4.0;\n"
			"		gl_TessLevelOuter[1] = 4.0;\n"
			"		gl_TessLevelOuter[2] = 4.0;\n"
			"	}"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"}"
		};

		const char* tse_source
		{
			"#version 450 core\n"
			"layout(triangles) in;\n"

			"layout(location = 0) uniform mat4 M;\n"
			
			"void main()\n"
			"{\n"
			"	gl_Position = gl_TessCoord.x * gl_in[0].gl_Position + gl_TessCoord.y * gl_in[1].gl_Position + gl_TessCoord.z * gl_in[2].gl_Position;\n"
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

		if (!Get_Shader_Status(vs, GL_COMPILE_STATUS))
			Get_Compile_Info(vs);

		GLuint tsc = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tsc, 1, &tsc_source, nullptr);
		glCompileShader(tsc);

		if (!Get_Shader_Status(tsc, GL_COMPILE_STATUS))
			Get_Compile_Info(tsc);

		GLuint tse = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tse, 1, &tse_source, nullptr);
		glCompileShader(tse);

		if (!Get_Shader_Status(tse, GL_COMPILE_STATUS))
			Get_Compile_Info(tse);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source, nullptr);
		glCompileShader(fs);

		if (!Get_Shader_Status(fs, GL_COMPILE_STATUS))
			Get_Compile_Info(fs);

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, tsc);
		glAttachShader(program, tse);
		glAttachShader(program, fs);

		glLinkProgram(program);

		if (!Get_Program_Status(program, GL_LINK_STATUS))
			Get_Link_Info(program);

		vec2 v[]
		{
			vec2(-0.5f), vec2(0.5f, -0.5f), vec2(0.0f, 0.5f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void Update() override
	{
		glUseProgram(program);
		static float time{ 0.0f };
		time += Time::deltaTime;
		mat4 M;
		M = rotate(M, time, vec3(0, 1, 0));
		glUniformMatrix4fv(0, 1, GL_FALSE, &M[0][0]);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_PATCHES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &vertex_array);
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
	GLuint program;
	GLuint vertex_array;
	GLuint vertex_buffer;
};

#if 0
CORE_MAIN(tessellation_triangle)
#endif
#include "../Classes/Renderer/Core.h"

#include <iostream>

class shader_status
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"void main()\n"
			"{\n"
			"	vec4 vertex[3] = vec4[3]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.0, 1.0),"
			"		vec4(0.5, -0.5, 0.0, 1.0),"
			"		vec4(0.0, 0.5, 0.0, 1.0)"
			"	);\n"
			"	gl_Position = vertex[gl_VertexID];\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(0.0, 1.0, 0.0, 0.0);\n"
			"}"
		};

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vs_source, nullptr);
		glCompileShader(vs);

		std::cout << Get_Shader_Status(vs, GL_SHADER_TYPE) << std::endl;
		std::cout << Get_Shader_Status(vs, GL_DELETE_STATUS) << std::endl;
		std::cout << Get_Shader_Status(vs, GL_SHADER_SOURCE_LENGTH) << std::endl;

		if (!Get_Shader_Status(vs, GL_COMPILE_STATUS))
			Get_Compile_Info(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source, nullptr);
		glCompileShader(fs);

		std::cout << Get_Shader_Status(fs, GL_SHADER_TYPE) << std::endl;
		std::cout << Get_Shader_Status(fs, GL_DELETE_STATUS) << std::endl;
		std::cout << Get_Shader_Status(fs, GL_SHADER_SOURCE_LENGTH) << std::endl;

		if (!Get_Shader_Status(fs, GL_COMPILE_STATUS))
			Get_Compile_Info(fs);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vs);
		glAttachShader(shader_program, fs);
		glLinkProgram(shader_program);

		std::cout << Get_Program_Status(shader_program, GL_DELETE_STATUS) << std::endl;
		std::cout << Get_Program_Status(shader_program, GL_ATTACHED_SHADERS) << std::endl;
		std::cout << Get_Program_Status(shader_program, GL_ACTIVE_ATTRIBUTES) << std::endl;
		std::cout << Get_Program_Status(shader_program, GL_ACTIVE_UNIFORMS) << std::endl;
		std::cout << Get_Program_Status(shader_program, GL_ACTIVE_UNIFORM_BLOCKS) << std::endl;

		if (!Get_Program_Status(shader_program, GL_LINK_STATUS))
			Get_Link_Info(shader_program);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	virtual void Update() override
	{
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
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
	GLuint shader_program;
};

#if 0
CORE_MAIN(shader_status)
#endif
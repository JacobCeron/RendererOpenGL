#include "Shader.h"

#include <iostream>

Shader::Shader()
{
	vs_program = glCreateProgram();
	glProgramParameteri(vs_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	tcs_program = glCreateProgram();
	glProgramParameteri(tcs_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	tes_program = glCreateProgram();
	glProgramParameteri(tes_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	gs_program = glCreateProgram();
	glProgramParameteri(gs_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	fs_program = glCreateProgram();
	glProgramParameteri(fs_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	cs_program = glCreateProgram();
	glProgramParameteri(cs_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	glCreateProgramPipelines(1, &program_pipeline);
}

void Shader::addShader(const char* shader_source, ShaderType type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_source, nullptr);
	glCompileShader(shader);
	
	if (!getShaderStatus(shader, ShaderStatus::COMPILE_STATUS))
		getCompileInfo(shader);

	GLuint program{ switchProgram(type) };

	glAttachShader(program, shader);
	glLinkProgram(program);

	if (!getProgramStatus(program, ShaderStatus::LINK_STATUS))
		getLinkInfo(program);

	ShaderStage stage{ switchStage(type) };
	glUseProgramStages(program_pipeline, stage, program);

	glDeleteShader(shader);
}

void Shader::changeShader(const char* shader_name, ShaderType type)
{
}

void Shader::use()
{
	glBindProgramPipeline(program_pipeline);
}

void Shader::delete_shader()
{
	glDeleteProgram(vs_program);
	glDeleteProgram(tcs_program);
	glDeleteProgram(tes_program);
	glDeleteProgram(gs_program);
	glDeleteProgram(fs_program);
	glDeleteProgram(cs_program);
	glDeleteProgramPipelines(1, &program_pipeline);
}

void Shader::setUniformi(ShaderType type, GLchar* name, GLint value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform1i(program, glGetUniformLocation(program, name), value);
}

void Shader::setUniformi(ShaderType type, GLint name, GLint value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform1i(program, name, value);
}

void Shader::setUniformf(ShaderType type, GLchar* name, GLfloat value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform1f(program, glGetUniformLocation(program, name), value);
}

void Shader::setUniformf(ShaderType type, GLint name, GLfloat value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform1f(program, name, value);
}

void Shader::setUniform2v(ShaderType type, GLchar* name, const vec2& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform2fv(program, glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setUniform2v(ShaderType type, GLint name, const vec2& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform2fv(program, name, 1, &value[0]);
}

void Shader::setUniform3v(ShaderType type, GLchar* name, const vec3& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform3fv(program, glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setUniform3v(ShaderType type, GLint name, const vec3& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform3fv(program, name, 1, &value[0]);
}

void Shader::setUniform4v(ShaderType type, GLchar* name, const vec4& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform4fv(program, glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setUniform4v(ShaderType type, GLint name, const vec4& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniform4fv(program, name, 1, &value[0]);
}

void Shader::setUniformMatrix4f(ShaderType type, GLchar* name, const mat4& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniformMatrix4fv(program, glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniformMatrix4f(ShaderType type, GLint name, const mat4& value)
{
	GLuint program{ switchProgram(type) };
	glProgramUniformMatrix4fv(program, name, 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::switchProgram(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:				return vs_program;	break;
	case ShaderType::TESS_CONTROL_SHADER:		return vs_program;	break;
	case ShaderType::TESS_EVALUATION_SHADER:	return vs_program;	break;
	case ShaderType::GEOMETRY_SHADER:			return vs_program;	break;
	case ShaderType::FRAGMENT_SHADER:			return vs_program;	break;
	case ShaderType::COMPUTE_SHADER:			return vs_program;	break;
	}
}

ShaderStage Shader::switchStage(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:				return ShaderStage::VERTEX_SHADER_STAGE;			break;
	case ShaderType::TESS_CONTROL_SHADER:		return ShaderStage::TESS_CONTROL_SHADER_STAGE;		break;
	case ShaderType::TESS_EVALUATION_SHADER:	return ShaderStage::TESS_EVALUATION_SHADER_STAGE;	break;
	case ShaderType::GEOMETRY_SHADER:			return ShaderStage::GEOMETRY_SHADER_STAGE;			break;
	case ShaderType::FRAGMENT_SHADER:			return ShaderStage::FRAGMENT_SHADER_STAGE;			break;
	case ShaderType::COMPUTE_SHADER:			return ShaderStage::COMPUTE_SHADER_STAGE;			break;
	}
}

GLint Shader::getShaderStatus(GLuint shader, ShaderStatus type)
{
	GLint status;
	glGetShaderiv(shader, type, &status);
	return status;
}

void Shader::getCompileInfo(GLuint shader)
{
	GLsizei info_log_length = getShaderStatus(shader, ShaderStatus::INFO_LOG_LENGTH);
	char* info_log = new char[info_log_length];
	glGetShaderInfoLog(shader, info_log_length, nullptr, info_log);
	std::cout << info_log << std::endl;
}

GLint Shader::getProgramStatus(GLuint program, ShaderStatus type)
{
	GLint status;
	glGetProgramiv(program, type, &status);
	return status;
}

void Shader::getLinkInfo(GLuint program)
{
	GLint info_log_length = getProgramStatus(program, ShaderStatus::INFO_LOG_LENGTH);
	char* info_log = new char[info_log_length];
	glGetProgramInfoLog(program, info_log_length, nullptr, info_log);
	std::cout << info_log << std::endl;
}
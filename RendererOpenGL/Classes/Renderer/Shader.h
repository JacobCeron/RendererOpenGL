#pragma once

#include <glad/glad.h>
#include "Enum/ShaderType.h"
#include "Enum/ShaderStatus.h"
#include "Enum/ShaderStage.h"
#include "../Maths/maths.h"

class Shader
{
public:
	Shader();
	void addShader(const char* shader_source, ShaderType type);
	void changeShader(const char* shader_name, ShaderType type);
	void use();
	void delete_shader();

	void setUniformi(ShaderType type, GLchar* name, GLint value);
	void setUniformi(ShaderType type, GLint name, GLint value);
	void setUniformf(ShaderType type, GLchar* name, GLfloat value);
	void setUniformf(ShaderType type, GLint name, GLfloat value);
	void setUniform2v(ShaderType type, GLchar* name, const vec2& value);
	void setUniform2v(ShaderType type, GLint name, const vec2& value);
	void setUniform3v(ShaderType type, GLchar* name, const vec3& value);
	void setUniform3v(ShaderType type, GLint name, const vec3& value);
	void setUniform4v(ShaderType type, GLchar* name, const vec4& value);
	void setUniform4v(ShaderType type, GLint name, const vec4& value);
	void setUniformMatrix4f(ShaderType type, GLchar* name, const mat4& value);
	void setUniformMatrix4f(ShaderType type, GLint name, const mat4& value);

private:
	GLuint switchProgram(ShaderType type);
	ShaderStage switchStage(ShaderType type);
	GLint getShaderStatus(GLuint shader, ShaderStatus type);
	void getCompileInfo(GLuint shader);
	GLint getProgramStatus(GLuint program, ShaderStatus type);
	void getLinkInfo(GLuint program);

private:
	GLuint vs_program;
	GLuint tcs_program;
	GLuint tes_program;
	GLuint gs_program;
	GLuint fs_program;
	GLuint cs_program;
	GLuint program_pipeline;
};
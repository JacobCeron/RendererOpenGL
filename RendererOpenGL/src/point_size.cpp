#include "../Classes/Renderer/Core.h"

#include <iostream>

class point_size
	: public Core
{
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			"layout(location = 0) in vec4 vPos;\n"

			"layout(std140, binding = 0) uniform TRANSFORM\n"
			"{\n"
			"	mat4 Model;\n"
			"	mat4 View;\n"
			"	mat4 Projection;\n"
			"} trans;\n"

			"layout(std430, binding = 0) buffer OUTPUT\n"
			"{\n"
			"	vec4 pos[];\n"
			"};\n"

			"layout(binding = 0) uniform atomic_uint counter;\n"
			"uint c;\n"

			"void main()\n"
			"{\n"
			"	c = atomicCounterIncrement(counter);\n"
			"	pos[gl_VertexID] = trans.Model * vPos;\n"
			"	if (gl_VertexID == 0)"
			"		gl_PointSize = 6.0;\n"
			"	gl_Position = trans.Projection * trans.View * pos[gl_VertexID];\n"
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

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source, nullptr);
		glCompileShader(fs);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vs);
		glAttachShader(shader_program, fs);
		glLinkProgram(shader_program);

		vec2 v[]{ vec2(-0.5f), vec2(0.5f, -0.5f), vec2(0.5f) };

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glUniformBlockBinding(shader_program, glGetUniformBlockIndex(shader_program, "TRANSFORM"), 6);
		glBindBufferBase(GL_UNIFORM_BUFFER, 6, uniform_buffer);

		mat4 View;
		View = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);

		mat4 *transform = (mat4*)glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		transform[0] = View;
		transform[1] = Projection;
		glUnmapNamedBuffer(uniform_buffer);

		glCreateBuffers(1, &storage_buffer);
		glNamedBufferStorage(storage_buffer, sizeof(vec4) * 3, nullptr, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storage_buffer);

		GLuint* counter = (GLuint*)glMapNamedBufferRange(storage_buffer, 0, sizeof(GLuint), GL_MAP_WRITE_BIT);
		*counter = 0;
		glUnmapNamedBuffer(storage_buffer);

		GLint points[2];
		glGetIntegerv(GL_POINT_SIZE_RANGE, points);
		std::cout << points[0] << " --- " << points[1] << std::endl;

		glEnable(GL_PROGRAM_POINT_SIZE);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		GLuint* counter = (GLuint*)glMapNamedBufferRange(storage_buffer, 0, sizeof(GLuint), GL_MAP_WRITE_BIT);
		*counter = 0;
		glUnmapNamedBuffer(storage_buffer);

		mat4 Model;
		Model = translate(Model, vec3(0.0f, 0.0f, -3.0f));
		Model = rotate(Model, time, vec3(0.0f, 1.0f, 0.0f));
		void* ptr = glMapNamedBufferRange(uniform_buffer, 0, sizeof(Model), GL_MAP_WRITE_BIT);
		memcpy(ptr, &Model, sizeof(Model));
		glUnmapNamedBuffer(uniform_buffer);

		ptr = glMapNamedBufferRange(storage_buffer, 0, sizeof(vec4) * 3, GL_MAP_READ_BIT);
		vec4 pos[3];
		memcpy(pos, ptr, sizeof(pos));
		std::cout << pos[0] << "\t\t" << pos[1] << "\t\t" << pos[2] << std::endl;
		glUnmapNamedBuffer(storage_buffer);

		glUseProgram(shader_program);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_POINTS, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteBuffers(1, &uniform_buffer);
		glDeleteBuffers(1, &storage_buffer);
	}

private:
	GLuint shader_program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint uniform_buffer;
	GLuint storage_buffer;
	GLuint atomic_counter; // Need fix because I never used it
};

#if 0
CORE_MAIN(point_size)
#endif
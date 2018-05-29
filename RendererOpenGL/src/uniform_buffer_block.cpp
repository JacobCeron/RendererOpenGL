#include "../Classes/Renderer/Core.h"
#include "../Classes/Maths/maths.h"

class uniform_buffer_block
	: public Core
{
public:
	uniform_buffer_block()
		: Core(800, 600, "OpenGL")
	{}

	virtual void Start() override
	{
		const char* vertex_source
		{
			"#version 450 core\n"
			"layout(std140, binding = 0) uniform TRANSFORMS\n"
			"{\n"
			"	mat4 Model;\n"
			"	mat4 View;\n"
			"	mat4 Projection;\n"
			"}trans;\n"
			"void main()\n"
			"{\n"
			"	vec4 vertices[3] = vec4[3]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.0, 0.5, 0.0, 1.0)\n"
			"	);\n"
			"	gl_Position = trans.Projection * trans.View * trans.Model * vertices[gl_VertexID];\n"
			"}"
		};

		const char* fragment_source
		{
			"#version 450 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0, 0.0, 0.0, 0.0);\n"
			"}"
		};

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
		glCompileShader(vertex_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
		glCompileShader(fragment_shader);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glUniformBlockBinding(shader_program, glGetUniformBlockIndex(shader_program, "TRANSFORMS"), 6);			// Use only if you don't supply a binding point in the uniform block declaration
		glBindBufferBase(GL_UNIFORM_BUFFER, 6, uniform_buffer);

		mat4 View;
		View = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 Projection;
		Projection = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		mat4 transforms[]{ View, Projection };
		void *ptr = glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(transforms), GL_MAP_WRITE_BIT);
		memcpy(ptr, &transforms[0], sizeof(transforms));
		glUnmapNamedBuffer(uniform_buffer);

		Window::current->setColor(vec4(0.5f));
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4 Model;
		Model = translate(Model, vec3(0.0f, 0.0f, -3.0f));
		Model = rotate(Model, time, vec3(0.0f, 1.0f, 1.0f));
		void *ptr = glMapNamedBufferRange(uniform_buffer, 0, sizeof(Model), GL_MAP_WRITE_BIT);
		memcpy(ptr, &Model, sizeof(Model));
		glUnmapNamedBuffer(uniform_buffer);

		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &uniform_buffer);
	}

private:
	GLuint shader_program;
	GLuint uniform_buffer;
};

#if 0
	CORE_MAIN(uniform_buffer_block)
#endif
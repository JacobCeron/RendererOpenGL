#include "../../Classes/Renderer/Core.h"
#include "../../Classes/Renderer/Shader.h"

#include <iostream>

class my_application
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"void main()\n"
			"{\n"
			"	vec4 vertices[3] = vec4[3]\n"
			"	(\n"
			"		vec4(-0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.5, -0.5, 0.0, 1.0),\n"
			"		vec4(0.0, 0.5, 0.0, 1.0)\n"
			"	);\n"
			"	gl_Position = vertices[gl_VertexID];\n"
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

		shader.addShader(vs_source, ShaderType::VERTEX_SHADER);
		shader.addShader(fs_source, ShaderType::FRAGMENT_SHADER);
	}

	virtual void Update() override
	{
		shader.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void End() override
	{
		shader.delete_shader();
	}

private:
	Shader shader;
};

#if 1
CORE_MAIN(my_application)
#endif
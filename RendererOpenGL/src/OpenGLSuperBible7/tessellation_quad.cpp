#include "../../Classes/Renderer/Core.h"

class tessellation_quad
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
			"};\n"

			"void main()\n"
			"{\n"
			"	gl_Position = vPos;\n"
			"}"
		};

		const char* tcs_source
		{
			"#version 450 core\n"
			"layout(vertices = 4) out;\n"

			"in gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_in[gl_MaxPatchVertices];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_out[];\n"

			"void main()\n"
			"{\n"
			"	if (gl_InvocationID == 0)\n"
			"	{\n"
			"		gl_TessLevelInner[0] = 9.0;\n"
			"		gl_TessLevelInner[1] = 7.0;\n"
			"		gl_TessLevelOuter[0] = 7.0;\n"
			"		gl_TessLevelOuter[1] = 9.0;\n"
			"		gl_TessLevelOuter[2] = 7.0;\n"
			"		gl_TessLevelOuter[3] = 9.0;\n"
			"	}\n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"}\n"
		};

		const char* tes_source
		{
			"#version 450 core\n"
			"layout(quads) in;\n"
			
			"in gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_in[gl_MaxPatchVertices];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out TSE_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}tse_out;\n"

			"void main()\n"
			"{\n"
			"	vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);\n"
			"	vec4 p1 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);\n"
			"	gl_Position = mix(p0, p1, gl_TessCoord.y);\n"
			"	tse_out.color = mix(p0, p1, gl_TessCoord.y);\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"

			"in TSE_OUT\n"
			"{\n"
			"	vec4 color;\n"
			"}fs_in;\n"
			
			"out vec4 FragColor;\n"
			
			"void main()\n"
			"{\n"
			"	FragColor = (fs_in.color + 0.5);\n"
			"}"
		};

		vs_program = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vs_source);
		tcs_program = glCreateShaderProgramv(GL_TESS_CONTROL_SHADER, 1, &tcs_source);
		tes_program = glCreateShaderProgramv(GL_TESS_EVALUATION_SHADER, 1, &tes_source);
		fs_program = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_source);

		glCreateProgramPipelines(1, &program_pipeline);
		glUseProgramStages(program_pipeline, GL_VERTEX_SHADER_BIT, vs_program);
		glUseProgramStages(program_pipeline, GL_TESS_CONTROL_SHADER_BIT, tcs_program);
		glUseProgramStages(program_pipeline, GL_TESS_EVALUATION_SHADER_BIT, tes_program);
		glUseProgramStages(program_pipeline, GL_FRAGMENT_SHADER_BIT, fs_program);

		vec2 v[]
		{
			vec2(-0.5f), vec2(0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void Update() override
	{
		glBindProgramPipeline(program_pipeline);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_PATCHES, 0, 4);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(tcs_program);
		glDeleteProgram(tes_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &vertex_array);
	}
	
private:
	GLuint vs_program;
	GLuint tcs_program;
	GLuint tes_program;
	GLuint fs_program;
	GLuint program_pipeline;
	GLuint vertex_buffer;
	GLuint vertex_array;
};

#if 0
CORE_MAIN(tessellation_quad)
#endif
#include "../../Classes/Renderer/Core.h"
#include "../../Classes/Renderer/stb_image.h"
#include "../../Classes/Renderer/Input.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

class tessellate_terrain
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"

			"layout(location = 0) in vec2 instance_position;\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}vs_out;\n"

			"layout(location = 0) uniform float terrain_level = 6.0;\n"

			"void main()\n"
			"{\n"

			"	vec3 v[4] = vec3[4]\n"
			"	(\n"
			"		vec3(-0.5, -0.5, -0.5),\n"
			"		vec3( 0.5, -0.5, -0.5),\n"
			"		vec3( 0.5f, 0.5, -0.5),\n"
			"		vec3(-0.5,  0.5, -0.5)\n"
			"	);\n"

			"	//int x = bitfieldExtract(gl_InstanceID, 0, 3);\n"
			"	//int y = bitfieldExtract(gl_InstanceID, 3, 3);\n"

			"	vec3 pos = v[gl_VertexID];\n"
			"	pos.xy += instance_position;\n"

			"	gl_Position = vec4(pos, 1.0);\n"
			"	vs_out.uv = (pos.xy + 0.5).xy / terrain_level;\n"
			"}"
		};

		const char* tcs_source
		{
			"#version 450 core\n"

			"layout(vertices = 4) out;\n"
			
			"in gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_in[];\n"

			"in VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}tcs_in[];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_out[];\n"

			"out TCS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}tcs_out[];\n"

			"layout(location = 6) uniform float tess_level = 64.0;\n"

			"void main()\n"
			"{\n"
			"	if (gl_InvocationID == 0)\n"
			"	{\n"
			"		gl_TessLevelInner[0] = tess_level;\n"
			"		gl_TessLevelInner[1] = tess_level;\n"

			"		gl_TessLevelOuter[0] = tess_level;\n"
			"		gl_TessLevelOuter[1] = tess_level;\n"
			"		gl_TessLevelOuter[2] = tess_level;\n"
			"		gl_TessLevelOuter[3] = tess_level;\n"
			"	}\n"

			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"	tcs_out[gl_InvocationID].uv = tcs_in[gl_InvocationID].uv;\n"
			"}"
		};

		const char* tes_source
		{
			"#version 450 core\n"

			"layout(quads) in;\n"
			
			"in gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_in[];\n"

			"in TCS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}tes_in[];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out TES_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}tes_out;\n"

			"layout(std140, binding = 0) uniform TRANSFORMS\n"
			"{\n"
			"	mat4 M;\n"
			"	mat4 V;\n"
			"	mat4 P;\n"
			"}transforms;\n"

			"layout(binding = 1) uniform sampler2D s;\n"
			"layout(location = 12) uniform float displacement_factor = 3.5;\n"
			"layout(location = 13) uniform float time;\n"

			"void main()\n"
			"{\n"
			"	vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);\n"
			"	vec4 p1 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);\n"
			"	vec4 pos = mix(p0, p1, gl_TessCoord.y);\n"

			"	vec2 uv0 = mix(tes_in[0].uv, tes_in[1].uv, gl_TessCoord.x);\n"
			"	vec2 uv1 = mix(tes_in[3].uv, tes_in[2].uv, gl_TessCoord.x);\n"
			"	vec2 uv = mix(uv0, uv1, gl_TessCoord.y);\n"

			"	pos.z += texture(s, uv + time).r * displacement_factor;\n"

			"	gl_Position = transforms.P * transforms.V * transforms.M * pos;\n"
			"	tes_out.uv = uv;\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"

			"in TES_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"}fs_in;\n"
			
			"out vec4 FragColor;\n"

			"layout(binding = 1) uniform sampler2D s;\n"
			"layout(location = 18) uniform float time;"
			
			"void main()\n"
			"{\n"
			"	FragColor = texture(s, fs_in.uv + time);\n"
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

		vec2* i_pos = new vec2[terrain_size * terrain_size];
		for (size_t y{ 0 }; y < terrain_size; y++)
			for (size_t x{ 0 }; x < terrain_size; x++)
				i_pos[x + y * terrain_size] = vec2(x, y);

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(vec2) * terrain_size * terrain_size, i_pos, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

		transforms* trans = (transforms*)glMapNamedBufferRange(uniform_buffer, 0, sizeof(mat4) * 3, GL_MAP_WRITE_BIT);
		trans->M = mat4();
		trans->V = lookAt(eye, target, up);
		trans->P = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);
		glUnmapNamedBuffer(uniform_buffer);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);
		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec2));
		glVertexArrayBindingDivisor(vertex_array, 0, 1);

		int width;
		int height;
		int channel;
		GLubyte* data = stbi_load("C:/Users/UserHp/Desktop/noise_map.png", &width, &height, &channel, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(1, texture);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void Update() override
	{
		static float time{ 0.0f };
		static float tess_level{ 30.0f };
		static float displacement_factor{ 3.0f };

		time += Time::deltaTime;
		if (Input::getKey(KeyCode::RightArrow))
			displacement_factor += 1.0f * Time::deltaTime;
		if (Input::getKey(KeyCode::LeftArrow))
			displacement_factor -= 1.0f * Time::deltaTime;
		if (Input::getKey(KeyCode::DownArrow))
			tess_level -= 1.0f * Time::deltaTime;
		if (Input::getKey(KeyCode::UpArrow))
			tess_level += 1.0f * Time::deltaTime;

		Camera_Move();

		static vec2 current_mouse_pos;
		static vec2 last_mouse_pos(Window::current->getWidth() / 2.0f, Window::current->getHeight() / 2.0f);

		current_mouse_pos = Input::mousePosition;
		vec2 angle_mouse = current_mouse_pos - last_mouse_pos;
		last_mouse_pos = current_mouse_pos;

		static float pitch{0.0f};
		static float yaw{90.0f};

		yaw += angle_mouse.x * Time::deltaTime;
		pitch += angle_mouse.y * Time::deltaTime;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		vec3 pos;
		pos.x = cos(radians(pitch)) * cos(radians(yaw));
		pos.y = sin(radians(pitch));
		pos.z = cos(radians(pitch)) * sin(radians(yaw));
		target = normalize(pos);

		transforms* trans = (transforms*)glMapNamedBufferRange(uniform_buffer, 0, sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		trans->M = mat4();
		trans->M = translate(trans->M, vec3(0.0f, -1.0f, 0.0f));
		trans->M = rotate(trans->M, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		trans->V = mat4();
		trans->V = lookAt(eye, eye + target, up);
		glUnmapNamedBuffer(uniform_buffer);

		glBindProgramPipeline(program_pipeline);

		glProgramUniform1f(vs_program, 0, terrain_size);
		glProgramUniform1f(tcs_program, 6, tess_level);
		glProgramUniform1f(tes_program, 12, displacement_factor);
		glProgramUniform1f(tes_program, 13, time * 0.1f);
		glProgramUniform1f(fs_program, 18, time * 0.1f);

		glBindVertexArray(vertex_array);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, terrain_size * terrain_size);
	}

	virtual void End() override
	{
		glDeleteProgram(vs_program);
		glDeleteProgram(tcs_program);
		glDeleteProgram(tes_program);
		glDeleteProgram(fs_program);
		glDeleteProgramPipelines(1, &program_pipeline);
	}

	void Camera_Move()
	{
		if (Input::getKey(KeyCode::A))
			eye -= normalize(cross(target, up)) * Time::deltaTime * speed;
		if (Input::getKey(KeyCode::D))
			eye += normalize(cross(target, up)) * Time::deltaTime * speed;
		if (Input::getKey(KeyCode::S))
			eye -= normalize(target) * Time::deltaTime * speed;
		if (Input::getKey(KeyCode::W))
			eye += normalize(target) * Time::deltaTime * speed;

		if (Input::getKey(KeyCode::Escape))
			Window::current->shouldClose(true);
	}

private:
	GLuint vs_program;
	GLuint tcs_program;
	GLuint tes_program;
	GLuint fs_program;
	GLuint program_pipeline;

	GLuint vertex_buffer;
	GLuint uniform_buffer;
	GLuint vertex_array;

	GLuint texture;

	size_t terrain_size = 66;
	float speed = 6.0f;

	vec3 eye = vec3(0.0f, 0.0f, 3.0f);
	vec3 target = vec3(0.0f, 0.0f, -1.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	struct transforms
	{
		mat4 M;
		mat4 V;
		mat4 P;
	};
};

#if 1
CORE_MAIN(tessellate_terrain)
#endif
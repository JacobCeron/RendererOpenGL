#include "../../Classes/Renderer/Core.h"
#include "../../Classes/Renderer/Shader.h"
#include "../../Classes/Renderer/Input.h"
#include "../../Classes/Renderer/stb_image.h"

#include <iostream>

class tessellation
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

			"out vec2 vs_uv;\n"

			"void main()\n"
			"{\n"
			"	vec3 v[4] = vec3[4]\n"
			"	(\n"
			"		vec3(-0.5, -0.5, 0.0),\n"
			"		vec3(0.5, -0.5, 0.0),\n"
			"		vec3(0.5, 0.5, 0.0),\n"
			"		vec3(-0.5, 0.5, 0.0)\n"
			"	);\n"

			"	vec3 vPos = v[gl_VertexID];\n"

			"	gl_Position = vec4(vPos, 1.0);\n"
			"	vs_uv = vPos.xy + 0.5;\n"
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

			"in vec2 vs_uv[];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_out[];\n"

			"out vec2 tcs_uv[];\n"

			"layout(location = 12) uniform float tess_level = 15.0f;\n"

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
			"	tcs_uv[gl_InvocationID] = vs_uv[gl_InvocationID];\n"
			"}"
		};

		const char* tes_source
		{
			"#version 450 core\n"
			
			"layout(quads) in;\n"

			"in gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"}gl_in[gl_MaxPatchVertices];\n"

			"in vec2 tcs_uv[];\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out vec2 uv;\n"

			"layout(location = 0) uniform mat4 M;\n"
			"layout(location = 1) uniform mat4 V;\n"
			"layout(location = 2) uniform mat4 P;\n"

			"layout(binding = 0) uniform sampler2D s;\n"
			"layout(location = 6) uniform float disp = 1.0;\n"

			"void main()\n"
			"{\n"
			"	vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);\n"
			"	vec4 p1 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);\n"
			"	vec4 pos = mix(p0, p1, gl_TessCoord.y);\n"

			"	vec2 uv0 = mix(tcs_uv[0], tcs_uv[1], gl_TessCoord.x);\n"
			"	vec2 uv1 = mix(tcs_uv[3], tcs_uv[2], gl_TessCoord.x);\n"
			"	vec2 tes_uv = mix(uv0, uv1, gl_TessCoord.y);\n"

			"	pos.z += texture(s, tes_uv).r * disp;\n"

			"	gl_Position = P * V * M * pos;\n"
			"	uv = tes_uv;\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"
			
			"in vec2 uv;\n"

			"out vec4 FragColor;\n"

			"layout(binding = 0) uniform sampler2D s;\n"

			"void main()\n"
			"{\n"
			"	FragColor = texture(s, uv) * vec4(0.0, 1.0, 0.0, 0.0);\n"
			"}"
		};

		shader.addShader(vs_source, ShaderType::VERTEX_SHADER);
		shader.addShader(tcs_source, ShaderType::TESS_CONTROL_SHADER);
		shader.addShader(tes_source, ShaderType::TESS_EVALUATION_SHADER);
		shader.addShader(fs_source, ShaderType::FRAGMENT_SHADER);

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		int width;
		int height;
		int channel;

		GLubyte* data = stbi_load("C:/Users/UserHp/Desktop/noise_map.png", &width, &height, &channel, 0);

		glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(0, texture);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Window::current->setColor(vec4(0.4f));
	}

	virtual void Update() override
	{
		static float disp{ 0.5f };
		static float tess_level{ 1.0f };
		static float time{ 0.0f };

		time += Time::deltaTime;

		if (Input::getKey(KeyCode::W))
			disp += 1.0f * Time::deltaTime;
		else if (Input::getKey(KeyCode::S))
			disp -= 1.0f * Time::deltaTime;

		if (Input::getKeyDown(KeyCode::A))
			tess_level -= 1.0f;
		else if (Input::getKeyDown(KeyCode::D))
			tess_level += 1.0f;

		mat4 M;
		M = scale(M, vec3(40.0f));
		M = translate(M, vec3(0.0f, 0.0f, -1.0f));
		M = rotate(M, time * 0.5f, vec3(0.0f, 1.0f, 0.0f));
		M = rotate(M, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		mat4 V;
		V = lookAt(vec3(0.0f, 25.0f, 30.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 P;
		P = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);

		shader.use();
		shader.setUniformf(ShaderType::TESS_CONTROL_SHADER, 12, tess_level);
		shader.setUniformMatrix4f(ShaderType::TESS_EVALUATION_SHADER, 0, M);
		shader.setUniformMatrix4f(ShaderType::TESS_EVALUATION_SHADER, 1, V);
		shader.setUniformMatrix4f(ShaderType::TESS_EVALUATION_SHADER, 2, P);
		shader.setUniformf(ShaderType::TESS_EVALUATION_SHADER, 6, disp);
		glDrawArrays(GL_PATCHES, 0, 4);
	}

	virtual void End() override
	{
		shader.delete_shader();
		glDeleteTextures(1, &texture);
	}

private:
	Shader shader;
	GLuint texture;
};

#if 0
CORE_MAIN(tessellation)
#endif
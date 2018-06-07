#include "../../Classes/Renderer/Core.h"
#include "../../Classes/Renderer/Shader.h"

#include <iostream>
#include "../../Classes/Renderer/stb_image.h"

class my_application
	: public Core
{
public:
	virtual void Start() override
	{
		const char* vs_source
		{
			"#version 450 core\n"
			
			"layout(location = 0) in vec4 vPos;\n"
			"layout(location = 1) in vec3 vNor;\n"

			"out gl_PerVertex\n"
			"{\n"
			"	vec4 gl_Position;\n"
			"};\n"

			"out VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"	vec3 N;\n"
			"	vec3 L;\n"
			"	vec3 E;\n"
			"}vs_out;\n"

			"layout(std140, binding = 0) uniform TRANSFORMS\n"
			"{\n"
			"	mat4 M;\n"
			"	mat4 V;\n"
			"	mat4 P;\n"
			"}transforms;"

			"layout(location = 0) uniform vec3 lightPos;\n"

			"void main()\n"
			"{\n"
			"	vec4 Pos = transforms.V * transforms.M * vPos;\n"
			
			"	vs_out.uv = vPos.xy + vPos.yz + vec2(0.5);\n"
			"	vs_out.N = mat3(transpose(inverse(transforms.V * transforms.M))) * vNor;\n"
			"	vs_out.L = lightPos - Pos.xyz;\n"
			"	vs_out.E = -Pos.xyz;\n"

			"	gl_Position = transforms.P * Pos;\n"
			"}"
		};

		const char* fs_source
		{
			"#version 450 core\n"

			"in VS_OUT\n"
			"{\n"
			"	vec2 uv;\n"
			"	vec3 N;\n"
			"	vec3 L;\n"
			"	vec3 E;\n"
			"}fs_in;\n"

			"out vec4 FragColor;\n"

			"layout(location = 1) uniform vec3 ambient_light;\n"
			"layout(location = 2) uniform vec3 ambient_material;\n"
			"layout(location = 3) uniform vec3 diffuse_light;\n"
			"layout(location = 4) uniform vec3 diffuse_material;\n"
			"layout(location = 5) uniform vec3 specular_light;\n"
			"layout(location = 6) uniform vec3 specular_material;\n"
			"layout(location = 7) uniform float shininess;\n"

			"layout(binding = 0) uniform sampler2D diffuse_texture;\n"
			"layout(binding = 1) uniform sampler2D specular_texture;\n"

			"void main()\n"
			"{\n"

			"	vec3 N = normalize(fs_in.N);\n"
			"	vec3 L = normalize(fs_in.L);\n"
			"	vec3 E = normalize(fs_in.E);\n"

			"	vec3 ambient = ambient_light * texture(diffuse_texture, fs_in.uv).xyz * ambient_material;\n"

			"	float diffuse_scale_factor = max(dot(L, N), 0.0);\n"
			"	vec3 diffuse = diffuse_scale_factor * diffuse_light * texture(diffuse_texture, fs_in.uv).xyz * diffuse_material;\n"

			"	vec3 R = reflect(-L, N);\n"
			"	float specular_scale_factor = pow(max(dot(E, R), 0.0), shininess);\n"
			"	vec3 specular = specular_scale_factor * specular_light * texture(specular_texture, fs_in.uv).xyz * specular_material;\n"

			"	vec3 rim = pow((1.0 - max(dot(E, N), 0.0)), 2.0) * vec3(0.0, 1.0, 0.0);\n"

			"	vec3 color = ambient + diffuse + specular + rim;\n"
			"	FragColor = vec4(color, 0.0);\n"
			"}"
		};

		shader.addShader(vs_source, ShaderType::VERTEX_SHADER);
		shader.addShader(fs_source, ShaderType::FRAGMENT_SHADER);

		vec3 v[]
		{
			vec3(-0.5f, -0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),
			vec3( 0.5f, -0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),
			vec3( 0.5f,  0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),
			vec3( 0.5f,  0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),
			vec3(-0.5f,  0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),
			vec3(-0.5f, -0.5f, -0.5f),		vec3(0.0f,  0.0f, -1.0f),

			vec3(-0.5f, -0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),
			vec3( 0.5f, -0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),
			vec3( 0.5f,  0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),
			vec3( 0.5f,  0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),
			vec3(-0.5f,  0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),
			vec3(-0.5f, -0.5f,  0.5f),		vec3(0.0f,  0.0f,  1.0f),

			vec3(-0.5f,  0.5f,  0.5f),		vec3(-1.0f, 0.0f,  0.0f),
			vec3(-0.5f,  0.5f, -0.5f),		vec3(-1.0f, 0.0f,  0.0f),
			vec3(-0.5f, -0.5f, -0.5f),		vec3(-1.0f, 0.0f,  0.0f),
			vec3(-0.5f, -0.5f, -0.5f),		vec3(-1.0f, 0.0f,  0.0f),
			vec3(-0.5f, -0.5f,  0.5f),		vec3(-1.0f, 0.0f,  0.0f),
			vec3(-0.5f,  0.5f,  0.5f),		vec3(-1.0f, 0.0f,  0.0f),

			vec3(0.5f,  0.5f,  0.5f),		vec3(1.0f,  0.0f,  0.0f),
			vec3(0.5f,  0.5f, -0.5f),		vec3(1.0f,  0.0f,  0.0f),
			vec3(0.5f, -0.5f, -0.5f),		vec3(1.0f,  0.0f,  0.0f),
			vec3(0.5f, -0.5f, -0.5f),		vec3(1.0f,  0.0f,  0.0f),
			vec3(0.5f, -0.5f,  0.5f),		vec3(1.0f,  0.0f,  0.0f),
			vec3(0.5f,  0.5f,  0.5f),		vec3(1.0f,  0.0f,  0.0f),

			vec3(-0.5f, -0.5f, -0.5f),		vec3(0.0f, -1.0f,  0.0f),
			vec3(0.5f, -0.5f, -0.5f),		vec3(0.0f, -1.0f,  0.0f),
			vec3(0.5f, -0.5f,  0.5f),		vec3(0.0f, -1.0f,  0.0f),
			vec3(0.5f, -0.5f,  0.5f),		vec3(0.0f, -1.0f,  0.0f),
			vec3(-0.5f, -0.5f,  0.5f),		vec3(0.0f, -1.0f,  0.0f),
			vec3(-0.5f, -0.5f, -0.5f),		vec3(0.0f, -1.0f,  0.0f),

			vec3(-0.5f,  0.5f, -0.5f),		vec3(0.0f,  1.0f,  0.0f),
			vec3(0.5f,  0.5f, -0.5f),		vec3(0.0f,  1.0f,  0.0f),
			vec3(0.5f,  0.5f,  0.5f),		vec3(0.0f,  1.0f,  0.0f),
			vec3(0.5f,  0.5f,  0.5f),		vec3(0.0f,  1.0f,  0.0f),
			vec3(-0.5f,  0.5f,  0.5f),		vec3(0.0f,  1.0f,  0.0f),
			vec3(-0.5f,  0.5f, -0.5f),		vec3(0.0f,  1.0f,  0.0f)
		};

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(v), v, GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &uniform_buffer);
		glNamedBufferStorage(uniform_buffer, sizeof(mat4) * 3, nullptr, GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

		mat4 V;
		V = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 P;
		P = perspective(radians(45.0f), static_cast<float>(Window::current->getWidth()) / Window::current->getHeight(), 0.1f, 100.0f);

		mat4* transforms = (mat4*)glMapNamedBufferRange(uniform_buffer, sizeof(mat4), sizeof(mat4) * 2, GL_MAP_WRITE_BIT);
		transforms[0] = V;
		transforms[1] = P;
		glUnmapNamedBuffer(uniform_buffer);

		glCreateVertexArrays(1, &vertex_array);
		glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexArrayAttrib(vertex_array, 0);
		glVertexArrayAttribBinding(vertex_array, 0, 0);

		glVertexArrayAttribFormat(vertex_array, 1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3));
		glEnableVertexArrayAttrib(vertex_array, 1);
		glVertexArrayAttribBinding(vertex_array, 1, 0);

		glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(vec3) * 2);

		int width;
		int height;
		int channel;

		glCreateTextures(GL_TEXTURE_2D, 2, texture);

		unsigned char* data = stbi_load("C:/Users/UserHp/Desktop/container.png", &width, &height, &channel, 0);
		glTextureStorage2D(texture[0], 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture[0], 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(0, texture[0]);

		data = stbi_load("C:/Users/UserHp/Desktop/container_specular.png", &width, &height, &channel, 0);
		glTextureStorage2D(texture[1], 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture[1], 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(1, texture[1]);

		glCreateSamplers(1, &sampler);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindSampler(0, sampler);
		glBindSampler(1, sampler);
	}

	virtual void Update() override
	{
		shader.use();

		static float time{ 0.0f };
		time += Time::deltaTime;

		mat4* M = (mat4*)(glMapNamedBufferRange(uniform_buffer, 0, sizeof(mat4), GL_MAP_WRITE_BIT));
		M[0] = translate(mat4(), vec3(0.0f, 0.0f, -3.0f));
		M[0] = rotate(M[0], time, vec3(1.0f, 1.0f, 1.0f));
		glUnmapNamedBuffer(uniform_buffer);

		shader.setUniform3v(ShaderType::VERTEX_SHADER, 0, vec3(0.0f, 0.0f, 1.0f));

		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 1, vec3(0.1f));
		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 2, vec3(1.0f, 1.0f, 1.0f));
		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 3, vec3(0.5f));
		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 4, vec3(1.0f, 1.0f, 1.0f));
		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 5, vec3(1.0f));
		shader.setUniform3v(ShaderType::FRAGMENT_SHADER, 6, vec3(1.0f, 1.0f, 1.0f));
		shader.setUniformf(ShaderType::FRAGMENT_SHADER, 7, 32);

		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	virtual void End() override
	{
		shader.delete_shader();
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &uniform_buffer);
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteTextures(2, texture);
	}

private:
	Shader shader;
	GLuint vertex_buffer;
	GLuint uniform_buffer;
	GLuint vertex_array;
	GLuint texture[2];
	GLuint sampler;
};

#if 0
CORE_MAIN(my_application)
#endif
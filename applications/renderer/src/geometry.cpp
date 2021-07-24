﻿#include "geometry.h"
#include "scene.h"
#include "stb/stb_image.h"

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
	DeletePrimitiveBuffers();
	DeletePrimitiveTextures();
}

void Geometry::SetName(const std::string& name)
{
	m_name = name;
}

void Geometry::SetMode(const std::string &name, GLenum mode)
{
	m_modes[name] = mode;
}

void Geometry::SetVertexArray(const std::string &name, std::vector<glm::vec3>& array)
{
	array.swap(m_positions[name]);
}

void Geometry::SetNormalArray(const std::string &name, std::vector<glm::vec3>& array)
{
	array.swap(m_normals[name]);
}

void Geometry::SetTexcoordArray(const std::string &name, std::vector<glm::vec2>& array)
{
	array.swap(m_texcoords[name]);
}

void Geometry::SetColorArray(const std::string &name, std::vector<glm::vec3>& array)
{
	array.swap(m_colors[name]);
}

void Geometry::SetTangentArray(const std::string &name, std::vector<glm::vec3>& array)
{
	array.swap(m_tangents[name]);
}

void Geometry::SetBitangentArray(const std::string &name, std::vector<glm::vec3>& array)
{
	array.swap(m_bitangents[name]);
}

void Geometry::CreatePrimitiveBuffers()
{
	for (const auto& positions_map : m_positions)
	{
		const auto& key = positions_map.first;

		const auto& positions = positions_map.second;

		if (positions.size() == 0)
		{
			continue;
		}

		const auto& normals = m_normals[key];
		const auto& texcoords = m_texcoords[key];
		const auto& colors = m_colors[key];
		const auto& tangents = m_tangents[key];
		const auto& bitangents = m_bitangents[key];

		auto& primitive_buffer = m_primitive_buffer_map[key];
		primitive_buffer.mode = m_modes[key];
		primitive_buffer.count = static_cast<GLsizei>(positions.size());

		glGenVertexArrays(1, &primitive_buffer.vao);
		glBindVertexArray(primitive_buffer.vao);

		// positions
		if (0 != positions.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_position);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_position);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		// normals
		if (0 != normals.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_normal);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_normal);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
		}

		// texcoords
		if (0 != texcoords.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_texcoord);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_texcoord);
			glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), &texcoords[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		// colors
		if (0 != colors.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_color);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_color);
			glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		// tangents
		if (0 != tangents.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_tangent);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_tangent);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		// bitangents
		if (0 != bitangents.size())
		{
			glGenBuffers(1, &primitive_buffer.vbo_bitangent);
			glBindBuffer(GL_ARRAY_BUFFER, primitive_buffer.vbo_bitangent);
			glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STREAM_DRAW);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Geometry::DeletePrimitiveBuffers()
{
	for (auto& primitive_buffer_map : m_primitive_buffer_map)
	{
		DeletePrimitiveBuffer(primitive_buffer_map.second);
	}
	m_primitive_buffer_map.clear();
}

void Geometry::SetTextures(const std::string &name, const TextureBuffer& texture_buffer)
{
	m_texture_buffer_map[name] = texture_buffer;
}

void Geometry::CreatePrimitiveTextures()
{
	for (auto& textures_map : m_texture_buffer_map)
	{
		auto& textures = textures_map.second;

		std::string file_name;

		file_name = textures.albedo.file_name;
		if (!file_name.empty())
		{
			if (textures.albedo.id != 0)
			{
				DeleteTexture(textures.albedo);
			}
			CreateTexture(textures.albedo);
		}

		file_name = textures.normal.file_name;
		if (!file_name.empty())
		{
			if (textures.normal.id != 0)
			{
				DeleteTexture(textures.normal);
			}
			CreateTexture(textures.normal);
		}

		file_name = textures.metallic.file_name;
		if (!file_name.empty())
		{
			if (textures.metallic.id != 0)
			{
				DeleteTexture(textures.metallic);
			}
			CreateTexture(textures.metallic);
		}

		file_name = textures.roughness.file_name;
		if (!file_name.empty())
		{
			if (textures.roughness.id != 0)
			{
				DeleteTexture(textures.roughness);
			}
			CreateTexture(textures.roughness);
		}

		file_name = textures.ao.file_name;
		if (!file_name.empty())
		{
			if (textures.ao.id != 0)
			{
				DeleteTexture(textures.ao);
			}
			CreateTexture(textures.ao);
		}

	}
}

void Geometry::DeletePrimitiveTextures()
{
	for (auto& textures_map : m_texture_buffer_map)
	{
		DeleteTextureBuffer(textures_map.second);
	}
	m_texture_buffer_map.clear();
}

void Geometry::SetMaterial(const std::string &name, const Material& material)
{
	m_material_map[name] = material;
}

void Geometry::SetShader(const std::shared_ptr<Shader>& shader)
{
	m_current_shader = shader;
}

void Geometry::Draw()
{
	if (m_current_shader == nullptr)
	{
		return;
	}

	m_current_shader->Bind();
	m_current_shader->LinkUniformMat4("model", m_model);

	for (auto& primitive_map : m_primitive_buffer_map)
	{
		const auto& key = primitive_map.first;
		const auto& primitive = primitive_map.second;

		const auto& texture_buffer = m_texture_buffer_map[key];
		glBindTextureUnit(0, texture_buffer.albedo.id);
		glBindTextureUnit(1, texture_buffer.normal.id);
		glBindTextureUnit(2, texture_buffer.metallic.id);
		glBindTextureUnit(3, texture_buffer.roughness.id);
		glBindTextureUnit(4, texture_buffer.ao.id);

		m_current_shader->LinkUniformBool("use_albedo_texture", false);
		m_current_shader->LinkUniformBool("use_normal_texture", false);
		m_current_shader->LinkUniformBool("use_metallic_texture", false);
		m_current_shader->LinkUniformBool("use_roughness_texture", false);
		m_current_shader->LinkUniformBool("use_ao_texture", false);

		if (texture_buffer.albedo.id != 0)
		{
			m_current_shader->LinkUniformBool("use_albedo_texture", true);
		}

		if (texture_buffer.normal.id != 0)
		{
			m_current_shader->LinkUniformBool("use_normal_texture", true);
		}

		if (texture_buffer.metallic.id != 0)
		{
			m_current_shader->LinkUniformBool("use_metallic_texture", true);
		}

		const auto& material = m_material_map[key];
		m_current_shader->LinkUniformVec3("material.ambient", material.ambient);
		m_current_shader->LinkUniformVec3("material.diffuse", material.diffuse);
		m_current_shader->LinkUniformVec3("material.specular", material.specular);
		m_current_shader->LinkUniformFloat("material.shininess", material.shininess);

		glBindVertexArray(primitive.vao);
		glDrawArrays(primitive.mode, primitive.first, primitive.count);
		glBindVertexArray(0);

		glBindTextureUnit(0, 0);
		glBindTextureUnit(1, 0);
		glBindTextureUnit(2, 0);
		glBindTextureUnit(3, 0);
		glBindTextureUnit(4, 0);
	}

	m_current_shader->Release();
}

void Geometry::UpdateBoundingBox(const std::vector<glm::vec3> &positions)
{
	m_center = glm::vec3(0.f, 0.f, 0.f);
	m_radius = 0.f;

	int count = static_cast<int>(positions.size());
	if (count == 0)
	{
		return;
	}

	int max_x = 0, max_y = 0, max_z = 0, min_x = 1, min_y = 1, min_z = 1;

	for (int i = 0; i < count; i++)
	{
		if (positions[i].x > max_x)
			max_x = i;
		if (positions[i].x < min_x)
			min_x = i;
		if (positions[i].y > max_y)
			max_y = i;
		if (positions[i].y < min_y)
			min_y = i;
		if (positions[i].z > max_z)
			max_z = i;
		if (positions[i].z < min_z)
			min_z = i;
	}

	glm::vec3 sub1, sub2;
	sub1.x = positions[max_x].x;
	sub1.y = positions[max_x].y;
	sub1.z = positions[max_x].z;
	sub2.x = positions[min_x].x;
	sub2.y = positions[min_x].y;
	sub2.z = positions[min_x].z;
	sub1 = sub1 - sub2;
	float x = glm::dot(sub1, sub1);

	sub1.x = positions[max_y].x;
	sub1.y = positions[max_y].y;
	sub1.z = positions[max_y].z;
	sub2.x = positions[min_y].x;
	sub2.y = positions[min_y].y;
	sub2.z = positions[min_y].z;
	sub1 = sub1 - sub2;
	float y = glm::dot(sub1, sub1);

	sub1.x = positions[max_z].x;
	sub1.y = positions[max_z].y;
	sub1.z = positions[max_z].z;
	sub2.x = positions[min_z].x;
	sub2.y = positions[min_z].y;
	sub2.z = positions[min_z].z;
	sub1 = sub1 - sub2;
	float z = glm::dot(sub1, sub1);

	float dia = 0;
	int max = max_x, min = min_x;
	if (z > x && z > y)
	{
		max = max_z;
		min = min_z;
		dia = z;
	}
	else if (y > x && y > z)
	{
		max = max_y;
		min = min_y;
		dia = y;
	}

	m_center.x = static_cast<float>(0.5 * (positions[max].x + positions[min].x));
	m_center.y = static_cast<float>(0.5 * (positions[max].y + positions[min].y));
	m_center.z = static_cast<float>(0.5 * (positions[max].z + positions[min].z));

	m_radius = static_cast<float>(0.5 * sqrt(dia));

	for (int i = 0; i < count; i++)
	{
		glm::vec3 d = positions[i] - m_center;
		float dist2 = glm::dot(d, d);

		if (dist2 > m_radius * m_radius)
		{
			float dist = static_cast<float>(sqrt(dist2));
			float new_radius = static_cast<float>((dist + m_radius) * 0.5);
			float k = static_cast<float>((new_radius - m_radius) / dist);
			m_radius = new_radius;

			glm::vec3 temp = d * k;
			m_center = m_center + temp;
		}
	}
}

void Geometry::DeletePrimitiveBuffer(PrimitiveBuffer &primitive_buffer)
{
	if (primitive_buffer.vao)
	{
		glDeleteVertexArrays(1, &primitive_buffer.vao);
	}

	if (primitive_buffer.vbo_position)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_position);
	}
	if (primitive_buffer.vbo_normal)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_normal);
	}
	if (primitive_buffer.vbo_texcoord)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_texcoord);
	}
	if (primitive_buffer.vbo_color)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_color);
	}
	if (primitive_buffer.vbo_tangent)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_tangent);
	}
	if (primitive_buffer.vbo_bitangent)
	{
		glDeleteBuffers(1, &primitive_buffer.vbo_bitangent);
	}

	primitive_buffer = PrimitiveBuffer();
}

void Geometry::CreateTexture(Texture &texture)
{
	if (texture.id != 0)
	{
		return;
	}

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(texture.is_vertical_flip);
	unsigned char *data = stbi_load(texture.file_name.c_str(), &width, &height, &nrComponents, 0);
	std::printf("Loading image: %s\n", texture.file_name.c_str());
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		texture.width = width;
		texture.height = height;

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load image file: " + texture.file_name);
	}
}

void Geometry::DeleteTextureBuffer(TextureBuffer &textures)
{
	if (textures.albedo.id != 0)
	{
		DeleteTexture(textures.albedo);
	}

	if (textures.normal.id != 0)
	{
		DeleteTexture(textures.normal);
	}

	if (textures.metallic.id != 0)
	{
		DeleteTexture(textures.metallic);
	}

	if (textures.roughness.id != 0)
	{
		DeleteTexture(textures.roughness);
	}

	if (textures.ao.id != 0)
	{
		DeleteTexture(textures.ao);
	}
}

void Geometry::DeleteTexture(Texture &texture)
{
	glDeleteTextures(1, &texture.id);
	std::memset(&texture, 0, sizeof(Texture));
}
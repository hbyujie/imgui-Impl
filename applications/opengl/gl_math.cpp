#include "gl_math.h"

void GLMath::ComputeBoundingSphereAverage(const std::vector<glm::vec3> &positions, glm::vec3 *center, float *radius)
{
    *center = glm::vec3(0.f, 0.f, 0.f);
    *radius = 0.f;

    int count = static_cast<int>(positions.size());

    for (int i = 0; i < count; i++)
    {
        *center += positions[i];
    }
    *center /= count;

    for (int i = 0; i < count; i++)
    {
        float distance = glm::distance(positions[i], *center);
        if (distance > *radius)
        {
            *radius = distance;
        }
    }
}

void GLMath::ComputeBoundingSphereRitter(const std::vector<glm::vec3> &positions, glm::vec3 *center, float *radius)
{
    *center = glm::vec3(0.f, 0.f, 0.f);
    *radius = 0.f;

    int count = static_cast<int>(positions.size());

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

    center->x = 0.5 * (positions[max].x + positions[min].x);
    center->y = 0.5 * (positions[max].y + positions[min].y);
    center->z = 0.5 * (positions[max].z + positions[min].z);

    *radius = 0.5 * sqrt(dia);

    for (int i = 0; i < count; i++)
    {
        glm::vec3 d = positions[i] - *center;
        float dist2 = glm::dot(d, d);

        if (dist2 > *radius * *radius)
        {
            float dist = sqrt(dist2);
            float new_radius = (dist + *radius) * 0.5;
            float k = (new_radius - *radius) / dist;
            *radius = new_radius;

            glm::vec3 temp = d * k;
            *center = *center + temp;
        }
    }
}

void GLMath::ComputeTangents(std::vector<Vertex>& vertices)
{
	const unsigned int vertex_size = vertices.size();
	assert(vertex_size % 3 == 0);

	for (unsigned int i = 0; i < vertex_size; i += 3)
	{
		unsigned int index1 = i;
		unsigned int index2 = i + 1;
		unsigned int index3 = i + 2;

		glm::vec3 pos1 = vertices[index1].position;
		glm::vec3 pos2 = vertices[index2].position;
		glm::vec3 pos3 = vertices[index3].position;

		glm::vec2 uv1 = vertices[index1].texcoord;
		glm::vec2 uv2 = vertices[index2].texcoord;
		glm::vec2 uv3 = vertices[index3].texcoord;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);

		vertices[index1].tangent = tangent;
		vertices[index2].tangent = tangent;
		vertices[index3].tangent = tangent;

		vertices[index1].bitangent = bitangent;
		vertices[index2].bitangent = bitangent;
		vertices[index3].bitangent = bitangent;
	}
}
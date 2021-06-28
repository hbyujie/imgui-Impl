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

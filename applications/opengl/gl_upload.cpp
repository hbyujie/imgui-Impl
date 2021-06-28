#include "gl_upload.h"

void GLUpload::UploadVertex(PartsMesh &parts_mesh)
{
    // vao
    glGenVertexArrays(1, &parts_mesh.vao);
    glBindVertexArray(parts_mesh.vao);

    // vbo
    glGenBuffers(1, &parts_mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, parts_mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, parts_mesh.vertices.size() * sizeof(Vertex), &parts_mesh.vertices[0], GL_STREAM_DRAW);

    // set attribPointer
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    // vertex colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    // vertex tangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    // vertex bitangents
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}